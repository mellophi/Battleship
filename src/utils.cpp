#include <utils.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fmt/core.h>

#define debug(x) printf("%s(%d): %lf", __func__, __LINE__, x);
#define N 320
#define p 12
#define frame 65

namespace utils{
	std::string project_root = "";

	void update_bar(int percent, const std::string &info)
	{
		int total = 100;
		int done = (percent / 100.0) * total;
		fmt::print("\r[");
		fmt::print("{:#>{}}", "", done);
		fmt::print("{:->{}}", "", total - done);
		fmt::print("]");
		fmt::print(" {:>2}% {}", percent, info);
		fflush(stdout);
	}

	//normalizing the input and giving an output file which contains the normalized output
	void normalize_input(FILE *input, FILE *output, int limit){
		double max = 0, dcShift = 0, sampleCount = 0;
		char singleLine[100];

		
		//calculate DC shift
		while(!feof(input)){
			fgets(singleLine, 100, input);
			if(!isalpha(singleLine[0])){
				double x = atof(singleLine);
				dcShift += x;
				sampleCount ++;
			}
		}
		dcShift /= sampleCount;
		//calculate normalization factor
		rewind(input);
		while(!feof(input)){
			fgets(singleLine, 100, input);
			if(!isalpha(singleLine[0])){
				double x = atof(singleLine);
				if(max < abs(x)){
					max = abs(x);
				}
			}
		}
		double normalizationFactor = abs(max - limit)/(max * 1.0);
		rewind(input);
		while(!feof(input)){
			fgets(singleLine, 100, input);
			if(!isalpha(singleLine[0])){
				double x = atof(singleLine);
				double normalized_output = (x-dcShift) * normalizationFactor;
				fprintf(output, "%lf\n", normalized_output);
			}
		}
		rewind(output);
	}

	//for applying the raised sin window on Ci's
	double raisedSin(int m){
		return 1+(p*1.0/2)*sin(3.14*m/12);
	}

	//for applying the hamming window on Si's
	double hammingWindow(int m){
		return (0.54-0.46*cos(2*3.14*m/N-1));
	}

	//calculating Ri's and outputing in a file
	void calculateR(double *s, FILE *output, double *r){
		for(int k=0; k<=p; ++k){
			for(int m=0; m<=N-k-1; ++m){
				r[k] += s[m]*s[m+k];
			}
			fprintf(output, "%lf\n", r[k]);
		}
	}

	//calculating Ri's for more than 1 frames and saving in a matrix
	void calculateR(double s[][N], double r[][p+1]){
		for(int f=1; f<=frame; ++f){
			for(int k=0; k<=p; ++k){
				for(int m=0; m<=N-k-1; ++m){
					r[f][k] += s[f][m]*s[f][m+k];
				}
			}
		}
	}

	//calculating Ai's and saving in a file
	void calculateA(double *r, FILE *output, double *a){
		double e[p+1] = {0}, k[p+1] = {0},  alpha[p+1][p+1] = {{0}};
		e[0] = r[0];
		for(int i=1; i<=p; ++i){
			double sum = 0;
			for(int j=1; j<=i-1; ++j){
				sum += alpha[i-1][j]*r[i-j];
			}
			k[i] = (r[i] - sum)/e[i-1];
			alpha[i][i] = k[i];
			for(int j=1; j<=i-1; ++j){
				alpha[i][j] = alpha[i-1][j] - k[i] * alpha[i-1][i-j];
			}
			e[i] = (1-k[i]*k[i])*e[i-1];
		}
		for(int i=1; i<=p; ++i){
			a[i] = alpha[p][i];
			fprintf(output, "%lf\n", a[i]);
		}
	}

	//calculating Ai's and saving in matrix for more than 1 frame
	void calculateA(double r[][p+1], double a[][p+1]){
		for(int f=1; f<=frame; ++f){
			double e[p+1] = {0}, k[p+1] = {0},  alpha[p+1][p+1] = {{0}};
			e[0] = r[f][0];
			for(int i=1; i<=p; ++i){
				double sum = 0;
				for(int j=1; j<=i-1; ++j){
					sum += alpha[i-1][j]*r[f][i-j];
				}
				k[i] = (r[f][i] - sum)/e[i-1];
				alpha[i][i] = k[i];
				for(int j=1; j<=i-1; ++j){
					alpha[i][j] = alpha[i-1][j] - k[i] * alpha[i-1][i-j];
				}
				e[i] = (1-k[i]*k[i])*e[i-1];
			}
			for(int i=1; i<=p; ++i){
				a[f][i] = alpha[p][i];
			}
		}
	}

	//calculating Ci's and saving in a file
	void calculateC(double sigma, double *a, FILE *output, double *c){
		c[0] = log10(sigma*sigma);
		for(int m=1; m<=p; ++m){
			double sum = 0;
			for(int k=1; k<=m-1; ++k){
				sum += (k/(m*1.0))*c[k]*a[m-k];
			}
			c[m] = a[m] + sum;
		}
		for(int i=0; i<=p; ++i){
			fprintf(output, "%lf\n", c[i]);
		}
	}

	//calculating Ci's and saving in a matrix after applying raised sin window on it for multiple frame
	void calculateC(double r[][p+1], double a[][p+1], double c[][p+1]){
		for(int f=1; f<=frame; ++f){
			double sigma = r[f][0];
			c[f][0] = log10(sigma*sigma);
			for(int m=1; m<=p; ++m){
				double sum = 0;
				for(int k=1; k<=m-1; ++k){
					sum += (k/(m*1.0))*c[f][k]*a[f][m-k];
				}
				c[f][m] = a[f][m] + sum;
			}
		}

		//applying raised sin
		for(int i=0; i<frame; ++i){
			for(int j=1; j<=p; ++j){
				c[i][j] *= raisedSin(j);
			}
		}
	}

	//skipping frames using frame count
	long skipFrames(FILE *input, long skipFrameCnt){
		long cnt = 0, pos;
		char singleLine[100];
		while(!feof(input)){
			fgets(singleLine, 100, input);
			if(!isalpha(singleLine[0]))
				cnt ++;
			if(cnt > skipFrameCnt){
				pos = ftell(input);
				break;
			}
		}
		return pos;
	}

	//to find the steady framees and save them in s
	void find_stable_frames(FILE *input, double s[][N], int x){
		char singleLine[1024];
		double samples[200][N] = {{0}}, energy[200] = {0};
		int sampleCnt = 0, frameCnt = 0, pos = 0;
		
		//reading the entire input and dividing them into frames and saving it in a
		//matrix (samples) of size number of frames x 320 (N)
		while(!feof(input)){
			fgets(singleLine, 1024, input);
			if(!isalpha(singleLine[0])){
				if(sampleCnt >= N){
					sampleCnt = 0;
					frameCnt ++;
				}
				samples[frameCnt][sampleCnt++] = atof(singleLine);
			}
		}

		//calculating STE for each of the frames and saving it in an array
		for(int i=0; i<frameCnt; ++i){
			double sum = 0;
			for(int j=0; j<N; ++j){
				sum += (samples[i][j] * samples[i][j]);
			}
			energy[i] = sum/(N * 1.0);
		}

		//finding out the frame with max STE and saving it in pos
		double maxEnergy = DBL_MIN;
		for(int i=0; i<frameCnt; ++i){
			if(energy[i] > maxEnergy){
				maxEnergy = energy[i];
				pos = i;
			}
		}

		//printf("Position of highest energy frame: %d+42=%d\n ", pos, pos+42);
		// taking start of the frames for analysis as pos - 2 till pos + 2
		// those frames are considered as stable frames and we are saving 
		// that in s for computation
		int upper = pos + x;
		pos = upper > 150 ? pos - (upper - 150) : pos ;

		frameCnt = 1;
		for(int i=pos-x; i<=pos+x; ++i){
			for(int j=0; j<N; ++j){
				s[frameCnt][j] = samples[i][j];
			}
			frameCnt++;
		}
	}
	// finding the tokhura's distance by reading the ci's of the reference file and ci's of the test
	// c_ref saves the reference ci's
	// c save the calculated ci's for test
	double tokhuraDistance(double c[][p+1], double w[], FILE *input){
		char singleLine[2048];
		double c_ref[frame][p+1] = {{0}};
		int framecnt = 0, cnt = 0;
		while(!feof(input) && cnt < 5){
			fgets(singleLine, sizeof(singleLine), input);
			sscanf(singleLine, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", &c_ref[cnt][0], &c_ref[cnt][1], &c_ref[cnt][2], &c_ref[cnt][3], &c_ref[cnt][4], &c_ref[cnt][5], &c_ref[cnt][6], &c_ref[cnt][7], &c_ref[cnt][8], &c_ref[cnt][9], &c_ref[cnt][10], &c_ref[cnt][11], &c_ref[cnt][12]);
			cnt++;
		}
		double finalDistance = 0;
		for(int i=0; i<frame; ++i){
			double distance = 0;
			for(int j=1; j<=p; ++j){
				double x = (c[i][j] - c_ref[i][j]);
				distance += w[j-1]*x*x;
			}
			finalDistance += (distance/(p*1.0));
		}
		return finalDistance/(frame * 1.0);
	}

	long double tokhura_distance(double c[p+1], long double codebook[p+1], long double w[p]){
		long double distance = 0; 
		for(int i=1; i<=p; ++i){
			long double x = (c[i] - codebook[i]);
			distance += w[i-1]*x*x;
		}
		return distance/(p*1.0);
	}

	// finding the euclidian's distance by reading the ci's of the reference file and ci's of the test
	// c_ref saves the reference ci's
	// c save the calculated ci's for test
	double euclidianDistance(double c[][p+1], FILE *input){
		char singleLine[2048];
		double c_ref[frame][p+1] = {{0}};
		int framecnt = 0, cnt = 0;
		while(!feof(input) && cnt < 5){
			fgets(singleLine, sizeof(singleLine), input);
			sscanf(singleLine, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", &c_ref[cnt][0], &c_ref[cnt][1], &c_ref[cnt][2], &c_ref[cnt][3], &c_ref[cnt][4], &c_ref[cnt][5], &c_ref[cnt][6], &c_ref[cnt][7], &c_ref[cnt][8], &c_ref[cnt][9], &c_ref[cnt][10], &c_ref[cnt][11], &c_ref[cnt][12]);
			cnt++;
		}
		double finalDistance = 0;
		for(int i=0; i<frame; ++i){
			double distance = 0;
			for(int j=1; j<=p; ++j){
				double x = (c[i][j] - c_ref[i][j]);
				distance += x*x;
			}
			finalDistance += (distance/(p*1.0));
		}
		return finalDistance/(frame * 1.0);
	}

	//utility function for applying hamming window
	void applyHamming(double s[][N]){
		for(int i=1; i<=frame; ++i){
			for(int j=0; j<N; ++j){
				s[i][j] *= hammingWindow(s[i][j]);
			}
		}

	}
};