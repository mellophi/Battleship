// Assignment-5.cpp : Defines the entry point for the console application.
//
#include <utils.h>
#include <HMM.h>
#include <cstring>
#include <fmt/core.h>
#define N 5 //no of states
#define M 32 //codebook size
#define T 65 //frame size or observation sequence size
#define SpF 320 //samples per frame
#define p 12
#define TRAINING_SEQUENCES 20
#define TESTING_SEQUENCES 10


static long double collect_A[TRAINING_SEQUENCES+1][N+1][N+1] = {{{0}}};
static long double collect_B[TRAINING_SEQUENCES+1][N+1][M+1] = {{{0}}};
static long double codebook[M+1][p] = {{0}};
static long double weight[p] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
static long double A[N+1][N+1] = {{0}};
static long double B[N+1][M+1] = {{0}}; 
static long double A_bar[N+1][N+1] = {{0}}; 
static long double B_bar[N+1][M+1] = {{0}}; 
static long double p_star = 0; 
static long double psi[N+1][T+1] = {{0}};
static long double delta[N+1][T+1] = {{0}}; 
static long double alpha[N+1][T+1] = {{0}}; 
static long double beta[N+1][T+1] = {{0}}; 
static long double gamma[N+1][T+1] = {{0}};
static long double zai[N+1][N+1][T+1] = {{{0}}};

static int observation[TRAINING_SEQUENCES+1][T+1] = {{0}}; 
static int pi[N+1] = {0};
static int pi_bar[N+1] = {0}; 
static int q_star[T+1] = {0};


static void average_lambda(){
	for(int i=1; i<=N; ++i){
		for(int j=1; j<=N; ++j){
			long double sum = 0;
			for(int k=1; k<=TRAINING_SEQUENCES; ++k){
				sum += collect_A[k][i][j];
			}
			A[i][j] = sum / (TRAINING_SEQUENCES * 1.0);
		}
	}


	for(int i=1; i<=N; ++i){
		for(int j=1; j<=M; ++j){
			for(int k=1; k<=TRAINING_SEQUENCES; ++k){
				B[i][j] += collect_B[k][i][j];
			}
			B[i][j] /= (TRAINING_SEQUENCES * 1.0);
		}
	}
}

static void collect_lambda(int seq){
	for(int i=1; i<=N; ++i){
		for(int j=1; j<=N; ++j){
			collect_A[seq][i][j] = A[i][j];
			
		}
		for(int j=1; j<=M; ++j){
			collect_B[seq][i][j] = B[i][j];
		}
	}

	//printf("------COLLECT A------\n");
	//for(int i=1; i<=TRAINING_SEQUENCES; ++i){
	//	for(int j=1; j<=N; ++j){
	//		for(int k=1; k<=N; ++k){
	//			printf("%g ", collect_A[i][j][k]);
	//		}
	//		printf("\n");
	//	}
	//	printf("------------------------\n");
	//}
}


static void print_lambda_to_file(int digit){
	printf("Saving the final model for %d\n", digit);
	char buffer[1024];
	sprintf(buffer, "%s/data/lambda/%d/A.txt", utils::project_root.c_str(), digit);
	FILE *output_A = fopen(buffer, "w");
	if(!output_A){
		printf("Cannot open file!\n");
		exit(0);
	}

	sprintf(buffer, "%s/data/lambda/%d/B.txt", utils::project_root.c_str(), digit);
	FILE *output_B = fopen(buffer, "w");
	if(!output_B){
		printf("Cannot open file!\n");
		exit(0);
	}

	sprintf(buffer, "%s/data/lambda/%d/pi.txt", utils::project_root.c_str(), digit);
	FILE *output_pi = fopen(buffer, "w");
	if(!output_pi){
		printf("Cannot open file!\n");
		exit(0);
	}

	for(int i=1; i<=N; ++i){
		for(int j=1; j<=N; ++j){
			fprintf(output_A, "%g ", A[i][j]);
		}

		for(int j=1; j<=M; ++j){
			fprintf(output_B, "%g ", B[i][j]);
		}

		fprintf(output_pi, "%d ", pi[i]);
		fprintf(output_A, "\n");
		fprintf(output_B, "\n");
	}

	fclose(output_A);
	fclose(output_B);
	fclose(output_pi);
}

static void print_lambda(){
	printf("------------A------------\n");
	for(int i=1; i<=N; ++i){
		for(int j=1; j<=N; ++j){
			printf("%g ", A[i][j]);
		}
		printf("\n");
	}
	printf("------------B------------\n");
	for(int i=1; i<=N; ++i){
		for(int j=1; j<=M; ++j){
			printf("%g ", B[i][j]);
		}
		printf("\n");
	}
	printf("------------PI------------\n");
	for(int i=1; i<=N; ++i){
		printf("%d ", pi[i]);
	}
	printf("\n");
}
//code for forward procedeure which returns the score of the model
static long double forward_procedure(int seq){
	//Initialization
	for(int i=1; i<=N; ++i){
		alpha[i][1] = pi[i] * B[i][observation[seq][1]];
	}

	//Induction
	for(int t=1; t<=T-1; ++t){
		for(int j=1; j<=N; ++j){
			long double sum = 0;
			for(int i=1; i<=N; ++i){
				sum += alpha[i][t] * A[i][j];
			}
			sum *= B[j][observation[seq][t+1]];
			alpha[j][t+1] = sum;
		}
	}

	//Termination
	long double score = 0;
	for(int i=1; i<=N; ++i)
		score += alpha[i][T];
	//printf("\n-----------ALPHA----------\n");
	//for(int i=1; i<=T; ++i){
	//	for(int j=1; j<=N; ++j){
	//		printf("%Le ", alpha[j][i]);
	//	}
	//	printf("\n");
	//}


	return score;
}

static void backward_procedure(int seq){
	
	//Initialization
	for(int i=1; i<=N; ++i)
		beta[i][T] = 1;

	//Induction
	for(int t=T-1; t>=1; --t){
		for(int i=1; i<=N; ++i){
			long double sum = 0;
			for(int j=1; j<=N; ++j){
				sum += A[i][j] * B[j][observation[seq][t+1]] * beta[j][t+1];
			}
			beta[i][t] = sum;
		}
	}

	//printf("\n-----------BETA----------\n");
	//for(int i=1; i<=T; ++i){
	//	for(int j=1; j<=N; ++j){
	//		printf("%g ", beta[j][i]);
	//	}
	//	printf("\n");
	//}
}

static long double viterbi(int seq){
	long double p_star = 0;
	//step 1: Initialization step
	for(int i=1;i<=N;i++){
		delta[i][1]=pi[i]*B[i][observation[seq][1]];
		psi[i][1]=0;
	}

	//step 2: Induction step
	long double max_delta=0;int maxi=0;
	for(int t=2;t<=T;t++){
		for(int j=1;j<=N;j++){
			max_delta=0;
			for(int i=1;i<=N;i++){
				if(max_delta<delta[i][t-1]*A[i][j]){
					max_delta=delta[i][t-1]*A[i][j];
					maxi=i;
				}
			}
			delta[j][t]=max_delta*B[j][observation[seq][t]];
				
			psi[j][t]=maxi;
		}
	}

	//step 3: Termination
	for(int i=1;i<=N;i++){
		if(p_star < delta[i][T]){
			p_star = delta[i][T];
			q_star[T] = i;
		}
	}

	//step 4: state sequence(Path) backtracking
	for(int t=T-1;t>=1;t--){
		q_star[t] = psi[q_star[t+1]][t+1];
	}

	return p_star;
}

static void calculate_gamma(){
	for(int t=1; t<=T; ++t){
		long double sum = 0;
		for(int j=1; j<=N; ++j)
			sum += alpha[j][t] * beta[j][t];
		for(int i=1; i<=N; ++i){
			gamma[i][t] = (alpha[i][t] * beta[i][t])/sum;
		}
	}

	//printf("\n-----------gamma----------\n");
	//for(int i=1; i<=T; ++i){
	//	for(int j=1; j<=N; ++j){
	//		printf("%g ", gamma[j][i]);
	//	}
	//	printf("\n");
	//}
}

static void reestimation(int seq){
	
	//Calculating zai
	for(int t=1; t<=T-1; ++t){
		long double sum = 0;
		for(int i=1; i<=N; ++i){
			for(int j=1; j<=N; ++j){
				sum += alpha[i][t] * A[i][j] * B[j][observation[seq][t+1]] * beta[j][t+1];
			}
		}
		for(int i=1; i<=N; ++i){
			for(int j=1; j<=N; ++j){
				zai[i][j][t] = (alpha[i][t] * A[i][j] * B[j][observation[seq][t+1]] * beta[j][t+1])/sum;
			}
		}
	}



	//printf("\n-----------gamma----------\n");
	//for(int i=1; i<=N; ++i){
	//	for(int j=1; j<=1; ++j){
	//		printf("%g ", gamma[i][j]);
	//	}
	//	printf("\n");
	//}

	//Calculating pi_bar
	for(int i=1; i<=N; ++i){
		pi_bar[i] = (int)gamma[i][1];
	}

// 	printf("\n*********PI_BAR************\n");
//    for (int i = 1; i <= 5; ++i) {
//            printf("%d,",pi_bar[i]);
// 	}

	//Calculating A_bar
	for(int i=1; i<=N; ++i){
		for(int j=1; j<=N; ++j){
			long double numerator = 0, denominator = 0;
			for(int t=1; t<=T-1; ++t){
				numerator += zai[i][j][t];
				denominator += gamma[i][t];
			}
			A_bar[i][j] = numerator / denominator;
		}
	}

	//Calculating B_bar
	for(int i=1; i<=N; ++i){
		for(int k=1; k<=M; ++k){
			long double numerator = 0, denominator = 0;
			for(int t=1; t<=T; ++t){
				if(observation[seq][t] == k){
					numerator += gamma[i][t];
				}
				denominator += gamma[i][t];
			}
			B_bar[i][k] = numerator / denominator;
		}
	}
}

static void initialize_bakis(){
	pi[1] = 1; // initializing pi
	
	for(int i=1; i<=N; ++i){

		//initializing B and A
		for(int j=1; j<=M; ++j){
			B[i][j] = (1/32.0);
		}

		if(i != 5){
			A[i][i] = 0.75;
			A[i][i+1] = 0.25;
		}

		else
			A[i][i] = 1;
	}
}

static void make_stocastic(){
		for (int i=1; i<=N; ++i) {
            for (int j=1; j<=N ; ++j) {
                A[i][j]=A_bar[i][j];
            }
            for (int j=1; j<=M; ++j) {
                B[i][j]=B_bar[i][j];
 
            }
            pi[i]=pi_bar[i];
        }

	//checking for A
	for(int i=1; i<=N; ++i){
		long double sum_row = 0, max_row = DBL_MIN;
		int max_pos = -1;
		for(int j=1; j<=N; ++j){
			sum_row += A[i][j];
			if(max_row < A[i][j]){ max_row = A[i][j]; max_pos = j; }
		}
		A[i][max_pos] -= sum_row - 1;
	}

	//checking for B
	for(int i=1; i<=N; ++i){
		long double sum_row = 0, max_row = DBL_MIN, cnt = 0;
		int max_pos = -1;
		for(int j=1; j<=M; ++j){
			cnt += !B[i][j];
			B[i][j] = !B[i][j] ? 1e-30 : B[i][j];
			if(max_row < B[i][j]) { max_row = B[i][j]; max_pos = j; }
			sum_row += B[i][j];
		}
		B[i][max_pos] -= (sum_row - (cnt * 1e-30)) - 1;
	}
}

void HMM::read_observation_sequence(int digit, const stage &choice){
	char buffer[1024];
	if(choice == stage::TRAIN){
		sprintf(buffer, "%s/data/output/train/obs_seq_%d.txt", utils::project_root.c_str(), digit);
		FILE *observation_file = fopen(buffer,"r"); //open input file in read mode
		for(int i=1; i<=TRAINING_SEQUENCES; ++i){
			for(int j=1; j<=T; ++j){
				fscanf(observation_file, "%d", &observation[i][j]);
			}
		}
		fclose(observation_file);
	}
	else if(choice == stage::TEST){
		sprintf(buffer, "%s/data/output/test/obs_seq_%d.txt", utils::project_root.c_str(), digit);
		FILE *observation_file = fopen(buffer,"r"); //open input file in read mode
		for(int i=1; i<=TESTING_SEQUENCES; ++i){
			for(int j=1; j<=T; ++j){
				fscanf(observation_file, "%d", &observation[i][j]);
			}
		}
		fclose(observation_file);
	}
}

static void read_lambda(int digit){
	char buffer[1024];
	sprintf(buffer, "%s/data/lambda/%d/A.txt", utils::project_root.c_str(), digit);
	FILE* input_A = fopen(buffer, "r");
	if(!input_A){
		printf("Cannot read file!\n");
		exit(0);
	}

	sprintf(buffer, "%s/data/lambda/%d/B.txt", utils::project_root.c_str(), digit);
	FILE* input_B = fopen(buffer, "r");
	if(!input_B){
		printf("Cannot read file!\n");
		exit(0);
	}

	sprintf(buffer, "%s/data/lambda/%d/pi.txt", utils::project_root.c_str(), digit);
	FILE *input_pi = fopen(buffer, "r");
	if(!input_pi){
		printf("Cannot read file!\n");
		exit(0);
	}

	for(int i=1; i<=N; ++i){
		for(int j=1; j<=N; ++j){
			fscanf(input_A, "%lf", &A[i][j]);
		}
		for(int j=1; j<=M; ++j){
			fscanf(input_B, "%lf", &B[i][j]);
		}
		fscanf(input_pi, "%d", &pi[i]);
	}

	//print_lambda();

	fclose(input_A);
	fclose(input_B);
	fclose(input_pi);
}


void HMM::converge(int digit){
	read_observation_sequence(digit, stage::TRAIN);
	//printf("-----------OBSERVATION SEQ-----------\n");
	//for(int i=1; i<=TRAINING_SEQUENCES; ++i){
	//	for(int j=1; j<=T; ++j){
	//		printf("%d ", observation[i][j]);
	//	}
	//	printf("\n");
	//}

	long double score = 0;
	for(int seq=1; seq<=TRAINING_SEQUENCES; ++seq){
		initialize_bakis();
		double prev_p_star = 0;
		for(int itr=1; itr<=200; itr++){
			if(p_star < prev_p_star) break;
			score = forward_procedure(seq);

			backward_procedure(seq);

			calculate_gamma();

			p_star = viterbi(seq);

			reestimation(seq);

			//Reseting the original lambda
			make_stocastic();
		}
		//print_lambda();
		collect_lambda(seq);
	}
	average_lambda();
	//print_lambda();	
	print_lambda_to_file(digit);
}

int HMM::test(int digit){
	int count = 0;
	for(int seq=1; seq<=TESTING_SEQUENCES; ++seq){
		long double max_score = DBL_MIN;
		int output_digit = -1;
		for(int i=0; i<10; ++i){
			read_lambda(i);
			//print_lambda();
			long double score = forward_procedure(seq);
			//printf("Score = %g\n", score);
			if(max_score < score) { max_score = score; output_digit = i; }
		}

		printf("Output for this sequence is %d\n", output_digit);
		count += digit == output_digit;
	}
	printf("Accuracy is %d%\n", count*10);
	return count;
}

static void read_codebook(){
	char buffer[1024];
	sprintf(buffer, "%s/data/codebook.txt", utils::project_root.c_str());
	//sprintf(buffer, "codebook.txt");
	FILE *codebook_file = fopen(buffer, "r");
	if(!codebook_file){
		printf("Cannot open the file!\n");
		exit(0);
	}

	for (int i = 1; i <= M; ++i)
		for (int j = 1; j <= p; ++j)
			fscanf(codebook_file,"%lf", &codebook[i][j]);
	fclose(codebook_file);

	// printf("-----------CODEBOOK-----------\n");
	// for(int i=1; i<=M; ++i){
	// 	for(int j=0; j<p; ++j){
	// 		printf("%g ", codebook[i][j]);
	// 	}
	// 	printf("\n");
	// }
}

static void print_observation_to_file(int digit, const stage &choice){
	char buffer[1024];


	if(choice == stage::TRAIN){
		sprintf(buffer, "%s/data/output/train/obs_seq_%d.txt", utils::project_root.c_str(), digit);
		FILE *observation_file = fopen(buffer, "w");
		if(!observation_file){
			printf("Cannot open file!\n");
			exit(0);
		}
		for(int i=1; i<=TRAINING_SEQUENCES; ++i){
			for(int j=1; j<=T; ++j){
				fprintf(observation_file, "%d ", observation[i][j]);
			}
			fprintf(observation_file, "\n");
		}
		fclose(observation_file);
		utils::update_bar((digit+1)*10, fmt::format("[obs_seq_{}.txt]", digit));
		// printf("Observation sequence saved in file %s\n\n", buffer);
	}

	if(choice == stage::TEST){
		sprintf(buffer, "%s/data/output/test/obs_seq_%d.txt", utils::project_root.c_str(), digit);
		FILE *observation_file = fopen(buffer, "w");
		if(!observation_file){
			printf("Cannot open file!\n");
			exit(0);
		}
		for(int i=1; i<=TESTING_SEQUENCES; ++i){
			for(int j=1; j<=T; ++j){
				fprintf(observation_file, "%d ", observation[i][j]);
			}
			fprintf(observation_file, "\n");
		}
		fclose(observation_file);
		utils::update_bar((digit+1)*10, fmt::format("[obs_seq_{}.txt]", digit));
		// printf("Observation sequence saved in file %s\n\n", buffer);
	}
}

void HMM::generate_observation_sequence(const stage &choice){
	read_codebook();
	//Training
	if(choice == stage::TRAIN){
		fmt::print("{:-^102}\n", " GENERATING OBSERVATION SEQUENCE FOR TRAINING STAGE ");
		for(int digit=0; digit<10; ++digit){
			// printf("Generating train observation sequence for digit %d .... \n", digit);
			for(int seq=1; seq<=TRAINING_SEQUENCES; ++seq){
				char filename[1024];
				sprintf(filename, "%s/data/recordings/%d/obs_%d.txt", utils::project_root.c_str(), digit, seq);
				FILE *input = fopen(filename, "r");
				FILE *input_normalized = fopen("normalized_input.txt", "w+");
		
				double stable_frames[T+1][SpF], r[T+1][p+1] = {{0}}, a[T+1][p+1] = {{0}}, c[T+1][p+1] = {{0}};

				utils::normalize_input(input, input_normalized);
		
				utils::find_stable_frames(input_normalized, stable_frames, 32);

				//for(int i=0; i<T; ++i){
				//	for(int j=0; j<SpF; ++j){
				//		printf("%g ", stable_frames[i][j]);
				//	}
				//	printf("\n");
				//}

				utils::applyHamming(stable_frames);

				utils::calculateR(stable_frames, r);

				utils::calculateA(r, a);

				utils::calculateC(r, a, c);

				read_codebook();

				for(int i=1; i<=T; ++i){
					long double min_distance = DBL_MAX;
					int min_index = -1;
					for(int j=1; j<=M; ++j){
						long double distance = utils::tokhura_distance(c[i], codebook[j], weight);
						//printf("Distance: %g Frame:%d Sequence:%d\n", distance, i, seq);
						if(min_distance > distance){
							min_distance = distance;
							min_index = j;
						}
					}
					observation[seq][i] = min_index;
				}

				//printf("---------------------------\n");

				//printf("--------C--------\n");
				//for(int i=1; i<=T; ++i){
				//	for(int j=1; j<=p; ++j){
				//		printf("%g ", c[i][j]);
				//	}
				//	printf("\n");
				//}

				fclose(input);
				fclose(input_normalized);

			}

			print_observation_to_file(digit, stage::TRAIN);

			//printf("--------OBSERVATION SEQUENCE FOR DIGIT %d--------\n", digit);
			//for(int i=1; i<=TRAINING_SEQUENCES; ++i){
			//	for(int j=1; j<=T; ++j){
			//		printf("%d ", observation[i][j]);
			//	}
			//	printf("\n");
			//}


		}
		fmt::print("\n");
	}
	else if(choice == stage::TEST){
		fmt::print("{:-^102}\n", " GENERATING OBSERVATION SEQUENCE FOR TESTING STAGE ");
		for(int digit=0; digit<10; ++digit){
			// printf("Generating test observation sequence for digit %d .... \n", digit);
			for(int seq=1; seq<=TESTING_SEQUENCES; ++seq){
				char filename[1024];
				sprintf(filename, "%s/data/testing/%d/obs_%d.txt", utils::project_root.c_str(), digit, seq);
				FILE *input = fopen(filename, "r");
				FILE *input_normalized = fopen("normalized_input.txt", "w+");
				
				double stable_frames[T+1][SpF], r[T+1][p+1] = {{0}}, a[T+1][p+1] = {{0}}, c[T+1][p+1] = {{0}};

				utils::normalize_input(input, input_normalized);
		
				utils::find_stable_frames(input_normalized, stable_frames, 32);

				//for(int i=0; i<T; ++i){
				//	for(int j=0; j<SpF; ++j){
				//		printf("%g ", stable_frames[i][j]);
				//	}
				//	printf("\n");
				//}

				utils::applyHamming(stable_frames);

				utils::calculateR(stable_frames, r);

				utils::calculateA(r, a);

				utils::calculateC(r, a, c);

				read_codebook();

				for(int i=1; i<=T; ++i){
					long double min_distance = DBL_MAX;
					int min_index = -1;
					for(int j=1; j<=M; ++j){
						long double distance = utils::tokhura_distance(c[i], codebook[j], weight);
						//printf("Distance: %g Frame:%d Sequence:%d\n", distance, i, seq);
						if(min_distance > distance){
							min_distance = distance;
							min_index = j;
						}
					}
					observation[seq][i] = min_index;
				}

				//printf("---------------------------\n");

				//printf("--------C--------\n");
				//for(int i=1; i<=T; ++i){
				//	for(int j=1; j<=p; ++j){
				//		printf("%g ", c[i][j]);
				//	}
				//	printf("\n");
				//}

				fclose(input);
				fclose(input_normalized);
			}

			print_observation_to_file(digit, stage::TEST);

			//printf("--------OBSERVATION SEQUENCE FOR DIGIT %d--------\n", digit);
			//for(int i=1; i<=TRAINING_SEQUENCES; ++i){
			//	for(int j=1; j<=T; ++j){
			//		printf("%d ", observation[i][j]);
			//	}
			//	printf("\n");
		}
		fmt::print("\n");
	}
}

