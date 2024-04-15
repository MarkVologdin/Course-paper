#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
// fs1 - z
// fs2 - x
// fs3 - y 

// разобратться с мерами и перевести все из градусов/час 
#define RadianToDegree  (180./M_PI)

#define D2R(Degrees)    ((Degrees)/RadianToDegree)          // Degree  --> Radians
#define M2R(Minutes)    ((Minutes/60.)/RadianToDegree)      // Minutes --> Radians
#define S2R(Seconds)    ((Seconds/3600.)/RadianToDegree)    // Seconds --> Radians
#define R2D(Radians)    ((Radians)*RadianToDegree)          // Radians --> Degree
#define R2M(Radians)    ((Radians)*RadianToDegree*60.)      // Radians --> Minutes
#define R2S(Radians)    ((Radians)*RadianToDegree*3600.)    // Radians --> Seconds
#define DH2RS(x)        (D2R(x)/3600.)
#define RS2DH(x)        (R2D(x)*3600.)
#define DMS2R(DD,MM,SS) (((DD)+(MM)/60.+(SS)/3600.)/RadianToDegree)
#define HMS2S(HH,MM,SS) ((SS)+(MM)*60+(HH)*3600.)

# define M_PI		3.14159265358979323846
double const  UE      = 7.2921151467e-5;      // Earth revolving coefficient 

double math_wait(double *arr,double * math_wait_arr,size_t len)
{   if(len == 0) return arr[0];
    return (len*math_wait_arr[len-1]+arr[len])/(len+1);
};

double math_dispersia(double *arr,double* math_dispersia_arr, double *math_wait, size_t len_arr, size_t len_math_wait)
{
    if(len_math_wait==0) return 1;
    if(len_arr == 1) { return sqrt(pow(arr[0]-arr[1],2))/2;};
    return sqrt((1-1/len_arr)*pow(math_dispersia_arr[len_arr-1],2)+pow(arr[len_arr]-math_wait[len_arr-1],2)/(len_arr+1));
};

size_t search_first_no_zero_elements( double * odo_data, size_t len)
{
    double epsilon = DBL_EPSILON;
    size_t count = 0;
    double a = odo_data[0];
    // printf("DBL epsilon =%.20f \n",epsilon);
    for (size_t i = 0; i < len ; i++)
    {   
        if(!(fabs(odo_data[i]-a)<=epsilon*fmax(fabs(odo_data[i]),fabs(a)))) break;
        count++;
    };
    
    return count;
}

size_t count_lines_in_file(FILE *file) {
    size_t line_count = 0;
    char ch;

    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            line_count++;
        }
    }

    rewind(file);

    return line_count;
}

struct Measurement {
    double* time;     // Time[s]
    double* fs1;      // Fs1[m/s/s]
    double* fs2;      // Fs2[m/s/s]
    double* fs3;      // Fs3[m/s/s]
    double* omega_s1; // omega_s1[d/h]
    double* omega_s2; // omega_s2[d/h]
    double* omega_s3; // omega_s3[d/h]
    double* odo;      // Odo[m]
    double* lon;      // Lon[d]
    double* lat;      // Lat[d]
    double* hei;      // Hei[m]
    double* ve;       // Ve[m/s/s]
    double* vn;       // Vn[m/s/s]
    double* vup;      // Vup[m/s/s]
    int* flag_gps;    // FlagGPS[]
};
void free_data( struct Measurement data)
{
    if (data.time != NULL){free(data.time);}
    if (data.fs1 != NULL){free(data.fs1);}
    if (data.fs2 != NULL){free(data.fs2);}
    if (data.fs3 != NULL){free(data.fs3);}
    if (data.omega_s1 != NULL){free(data.omega_s1);}
    if (data.omega_s2 != NULL){free(data.omega_s2);}
    if (data.omega_s3 != NULL){free(data.omega_s3);}
    if (data.odo != NULL){free(data.odo);}
    if (data.hei != NULL){free(data.hei);}
    if (data.lon != NULL){free(data.lon);}
    if (data.lat != NULL){free(data.lat);}
    if (data.ve != NULL){free(data.ve);}
    if (data.vn != NULL){free(data.vn);}
    if (data.vup != NULL){free(data.vup);}
    if (data.flag_gps != NULL){free(data.flag_gps);}

}

double search_mediums_sensors_mean(double * array, size_t len)
{   
    double sum = 0;
    for (size_t i = 0; i < len; i++)
    {
        sum += array[i]; 
    }
    
    return sum/len;
}
double* search_mediums_sensors_mean_all(double * array, size_t len)
{   
    double sum = 0;
    double * mean = (double*)malloc((len+1)*sizeof(double));
    mean[0] = array[0];
    for (size_t i = 0; i < len; i++)
    {
        sum += array[i]; 
        mean[i] = sum/i;
    };
    
    return mean;
}

double norma_vectora(double * arr)
{
    return sqrt(pow(arr[0],2)+pow(arr[1],2)+pow(arr[2],2));
}

double* vector_product(double* vect1, double* vect2, size_t len)
{   // for 3 dimentions
    double * new_vector = (double*)malloc(len*sizeof(double));
    new_vector[0] = vect1[1]*vect2[2]-vect2[1]*vect1[2];
    new_vector[1] = vect2[0]*vect1[2]-vect1[0]*vect2[2];
    new_vector[2] = vect1[0]*vect2[1]-vect2[0]*vect1[1];
    return new_vector;
}
double* search_angle_orientation_model_1(double * fs1_mean, double *fs2_mean, double * fs3_mean, size_t len)
{
    double theta , gamma; double *angles;
    
    angles = (double*)malloc(3*sizeof(double));
    
    theta = atan2(fs1_mean[len],(sqrt(pow(fs2_mean[len],2)+pow(fs3_mean[len],2))));// in radians
    gamma = -(atan2(fs3_mean[len],fs2_mean[len])); // in radians
    //printf("Calculos gamma with fs2_mean= %.6f, fs3_mean = %.6f, gamma[rad] = %.6f, gamma[d] = %.6f\n",fs2_mean[len], fs3_mean[len], gamma, R2D(gamma));
    angles[0] = 0;
    angles[1] = gamma;
    angles[2] = theta; 

    return angles;
}
double* search_angle_orientation_model_2_ortoganalization(double * fs1_mean, double *fs2_mean, double * fs3_mean, size_t len)
{
    double theta , gamma; double *angles; double *l3;

    l3 = (double*)malloc(3*sizeof(double));
    angles = (double*)malloc(3*sizeof(double));

    l3[0] = fs1_mean[len];
    l3[1] = fs2_mean[len];
    l3[2] = fs3_mean[len];
    
    
    l3[0] = l3[0]/norma_vectora(l3);
    l3[1] = l3[1]/norma_vectora(l3);
    l3[2] = l3[2]/norma_vectora(l3);

    theta = atan2(l3[0],(sqrt(pow(l3[1],2)+pow(l3[2],2))));// in radians
    gamma = -(atan2(l3[2],l3[1])); // in radians
    angles[0] = 0;
    angles[1] = gamma;
    angles[2] = theta; 

    return angles;
}
double * search_angle_psi(double * angles, double *omega_s1_mean,double *omega_s2_mean,double *omega_s3_mean, double* lat_mean, size_t len)
{
    double lat_mean_rad;
    double* columns_l2; double* columns_l3; double* columns_l1;

    columns_l3 = (double*)malloc(3*sizeof(double));
    columns_l2 = (double*)malloc(3*sizeof(double));

    columns_l3[0] = sin(angles[2]); // sin(radians), cos(radians)
    columns_l3[1] = cos(angles[2])*cos(angles[1]); //
    columns_l3[2] = -cos(angles[2])*sin(angles[1]);

    //lat_mean = search_mediums_sensors_mean(lat, len);
    lat_mean_rad = D2R(lat_mean[len]);
    
    
    columns_l2[0]= (DH2RS(omega_s1_mean[len])- columns_l3[0]*UE*sin(lat_mean_rad))/(UE*cos(lat_mean_rad));
    columns_l2[1]= (DH2RS(omega_s2_mean[len])- columns_l3[1]*UE*sin(lat_mean_rad))/(UE*cos(lat_mean_rad));
    columns_l2[2]= (DH2RS(omega_s3_mean[len])- columns_l3[2]*UE*sin(lat_mean_rad))/(UE*cos(lat_mean_rad));

    // printf("omega1=%.10f, columns*UE*sin=%.10f,columns=%.10f\n",DH2RS(omega_s1_mean[len]),columns_l3[0]*UE*sin(lat_mean_rad),columns_l3[0]);
    // printf("omega1=%.10f, columns*UE*sin=%.10f,columns=%.10f\n",DH2RS(omega_s2_mean[len]),columns_l3[0]*UE*sin(lat_mean_rad),columns_l3[0]);
    // printf("omega1=%.10f, columns*UE*sin=%.10f,columns=%.10f\n",DH2RS(omega_s3_mean[len]),columns_l3[0]*UE*sin(lat_mean_rad),columns_l3[0]);
    
    
    columns_l2[0] = columns_l2[0]/ norma_vectora(columns_l2);
    columns_l2[1] = columns_l2[1]/ norma_vectora(columns_l2);
    columns_l2[2] = columns_l2[2]/ norma_vectora(columns_l2);

    columns_l3[0] = columns_l3[0]/ norma_vectora(columns_l3);
    columns_l3[1] = columns_l3[1]/ norma_vectora(columns_l3);
    columns_l3[2] = columns_l3[2]/ norma_vectora(columns_l3);
    
    columns_l1 = vector_product(columns_l2, columns_l3, 3);

    angles[0] = atan2((columns_l1[0]),(columns_l1[1]));
    //printf("Calculus psi = %.6f\n", R2D(angles[0]));

    return angles;
}
double * search_angle_psi_2(double *f_medium, double * angles, double *medium_omega_s1,double *medium_omega_s2,double *medium_omega_s3, size_t len)
{
    
    double* columns_l2;
    columns_l2 = (double*)malloc(3*sizeof(double));

    double * medium_omega = (double*)malloc(3*sizeof(double));

    medium_omega[0] = medium_omega_s1[len];
    medium_omega[1] = medium_omega_s2[len];
    medium_omega[2] = medium_omega_s3[len];

    for (size_t i = 0; i < 3; i++)
    {
        columns_l2[i] = (vector_product(medium_omega, f_medium, 3)[i])/norma_vectora(vector_product(medium_omega, f_medium, 3));
    }

   // columns_l2 = vector_product(columns_l3, columns_l1,3);

    angles[0] = atan2((columns_l2[0]),(columns_l2[1]));

    return angles;
}

int main() {
    char filename[] = "INS_Odo_Data.txt";
    FILE *file = fopen(filename, "r");
    FILE *Ffile1 = fopen("output_angels_1.txt", "w+");
    FILE *Ffile3 = fopen("output_means.txt", "w+");
    FILE *Ffile2 = fopen("output_angels_2.txt", "w+");
    FILE *file_math_wait = fopen("output_math_wait", "w+");
    FILE *file_math_dispersia = fopen("output_math_dispersia", "w+");

    if (file == NULL) {
        perror("Error opening the file_INS_ODO ");
        return 1;
    }
    if (file_math_wait == NULL) {
        perror("Error opening the file_math");
        return 1;
    }
    if (file_math_dispersia== NULL) {
        perror("Error opening the file_math");
        return 1;
    }
    if (Ffile1 == NULL) {
        perror("Error opening the file_angels");
        return 1;
    }
    if (Ffile2 == NULL) {
        perror("Error opening the file_angels");
        return 1;
    }

    size_t num_lines = count_lines_in_file(file);
    // printf("%zu\n", num_lines);

    // getchar();
    size_t size_of_struct = sizeof(struct Measurement);
    size_t total_size = size_of_struct* num_lines;
    printf("Total size data = %zu\n", total_size);

    struct Measurement data; //(struct Measurement *)malloc(num_lines * sizeof(struct Measurement));
    // if (data == NULL) {
    //     perror("Memory allocation error");
    //     fclose(file);
    //     return 1;
    // }

    data.time = (double*)malloc(num_lines*sizeof(double));
    data.fs1 = (double*)malloc(num_lines*sizeof(double));
    data.fs2 = (double*)malloc(num_lines*sizeof(double));
    data.fs3 = (double*)malloc(num_lines*sizeof(double));
    data.omega_s1 = (double*)malloc(num_lines*sizeof(double));
    data.omega_s2 = (double*)malloc(num_lines*sizeof(double));
    data.omega_s3 = (double*)malloc(num_lines*sizeof(double));
    data.odo = (double*)malloc(num_lines*sizeof(double));
    data.lon = (double*)malloc(num_lines*sizeof(double));
    data.lat = (double*)malloc(num_lines*sizeof(double));
    data.hei = (double*)malloc(num_lines*sizeof(double));
    data.ve = (double*)malloc(num_lines*sizeof(double));
    data.vn = (double*)malloc(num_lines*sizeof(double));
    data.vup = (double*)malloc(num_lines*sizeof(double));
    data.flag_gps = (int*)malloc(num_lines*sizeof(int));


    char line[4096];
    char *token;
    int row = 0;

    // Skip the first line with column names
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Error reading the first line");
        free_data(data);
        fclose(file);
        return 1;
    }
  // Read and store the data
    while (fgets(line, sizeof(line), file) != NULL) {
        token = strtok(line, " \t"); // Assuming space or tab as the delimiter

        if (token == NULL) {
            continue; // Skip empty lines
        }

        data.time[row] = atof(token);
        token = strtok(NULL, " \t");
        data.fs3[row] = atof(token);
        token = strtok(NULL, " \t");
        data.fs1[row] = atof(token);
        token = strtok(NULL, " \t");
        data.fs2[row] = atof(token);
        token = strtok(NULL, " \t");
        data.omega_s3[row] = atof(token);
        token = strtok(NULL, " \t");
        data.omega_s1[row] = atof(token);
        token = strtok(NULL, " \t");
        data.omega_s2[row] = atof(token);
        token = strtok(NULL, " \t");
        data.odo[row] = atof(token);
        token = strtok(NULL, " \t");
        data.lon[row] = atof(token);
        token = strtok(NULL, " \t");
        data.lat[row] = atof(token);
        token = strtok(NULL, " \t");
        data.hei[row] = atof(token);
        token = strtok(NULL, " \t");
        data.ve[row] = atof(token);
        token = strtok(NULL, " \t");
        data.vn[row] = atof(token);
        token = strtok(NULL, " \t");
        data.vup[row] = atof(token);
        token = strtok(NULL, " \t");
        data.flag_gps[row] = atoi(token);

        row++;
    }
    for (size_t i = 0; i < 10; i++)
    {
        printf("N = %zu , Fs1 = %.6f, Fs2 = %.6f, Fs3 = %.6f \n", i, data.fs1[i], data.fs2[i], data.fs3[i]);
    }
    

    fclose(file);
    size_t number_no_zero_odo = search_first_no_zero_elements(data.odo, num_lines);
    printf("First no zero element = %zu\n", number_no_zero_odo);
    printf("%.6f ", data.time[number_no_zero_odo-2]);
    printf("%.6f\n", data.odo[number_no_zero_odo-2]);
    printf("%.6f ", data.time[number_no_zero_odo]);
    printf("%.6f\n", data.odo[number_no_zero_odo]);

    double *mean_fs1 = (double*)malloc(num_lines*sizeof(double));
    double *mean_fs2 = (double*)malloc(num_lines*sizeof(double));
    double *mean_fs3 = (double*)malloc(num_lines*sizeof(double));
    double *mean_omega_s1 = (double*)malloc(num_lines*sizeof(double));
    double *mean_omega_s2 = (double*)malloc(num_lines*sizeof(double));
    double *mean_omega_s3 = (double*)malloc(num_lines*sizeof(double));
    double * lat_mean = (double*)malloc(num_lines*sizeof(double));

    mean_fs1 = search_mediums_sensors_mean_all(data.fs1, number_no_zero_odo);
    mean_fs2 = search_mediums_sensors_mean_all(data.fs2, number_no_zero_odo);
    mean_fs3 = search_mediums_sensors_mean_all(data.fs3, number_no_zero_odo);
    mean_omega_s1 = search_mediums_sensors_mean_all(data.omega_s1, number_no_zero_odo);
    mean_omega_s2 = search_mediums_sensors_mean_all(data.omega_s2, number_no_zero_odo);
    mean_omega_s3 = search_mediums_sensors_mean_all(data.omega_s3, number_no_zero_odo);
    lat_mean = search_mediums_sensors_mean_all(data.lat, number_no_zero_odo);

    fprintf(Ffile3,"Time Fs1_mean Fs2_mean Fs3_mean Omega_s1 Omega_s2 Omega_s3");
    for (size_t i = 0; i < number_no_zero_odo; i++)
    {
        fprintf(Ffile3,"%.6f %.6f %.6f %.6f %.6f %.6f %.6f", data.time[i], mean_fs1[i],  mean_fs2[i],  mean_fs3[i], mean_omega_s1[i], mean_omega_s2[i], mean_omega_s3[i]);
    }
    

    size_t number_of_steps = number_no_zero_odo/600;

    double* angles1; double* angles2; 
    double * f_medium = (double*)malloc(3*sizeof(double)); double * columns_l3;

    columns_l3 = (double*)malloc(3*sizeof(double));

    fprintf(Ffile1, "№    Time    Psi    Gamma    Theta\n");
    fprintf(Ffile2, "№    Time    Psi    Gamma    Theta\n");
    printf("Number of steps=%zu,number no zero odo = %zu\n", number_of_steps, number_no_zero_odo);
    for (size_t i = 1; i < number_no_zero_odo; i+=1)
    {
        f_medium[0] = mean_fs1[i];
        f_medium[1] = mean_fs2[i];
        f_medium[2] = mean_fs3[i];

        if(i%5000 == 0){printf("Iteration = %zu\n", i);};
        angles1 = search_angle_orientation_model_1(mean_fs1,mean_fs2,mean_fs3, i); // means model orientation angles
        angles1 = search_angle_psi(angles1,mean_omega_s1, mean_omega_s2, mean_omega_s3,lat_mean, i);
        angles2 = search_angle_orientation_model_2_ortoganalization(mean_fs1,mean_fs2,mean_fs3, i); // means model orientation angles
        angles2 = search_angle_psi_2(f_medium,angles2, mean_omega_s1, mean_omega_s2,  mean_omega_s3, i);
        fprintf(Ffile1, "%zu    %.6f    %.6f    %.6f    %.6f \n", i, data.time[i], angles1[0]*180/M_PI, angles1[1]*180/M_PI, angles1[2]*180/M_PI);
        fprintf(Ffile2, "%zu    %.6f    %.6f    %.6f    %.6f \n", i, data.time[i], angles2[0]*180/M_PI, angles2[1]*180/M_PI, angles2[2]*180/M_PI);
    }
    fclose(Ffile1);
    fclose(Ffile2);
    // for (size_t i = 0; i < 3; i++)
    // {
    //     printf("Orientation angles[%zu] = %f \n",i,angles[i]*180/M_PI);
    // }
    
    // for (size_t i = 0; i < 3; i++)
    // {
    //     printf("Orientation version1 angles[%zu] = %f \n",i,angles1[i]*180/M_PI);
    //     printf("Orientation version2 angles[%zu] = %f \n",i,angles2[i]*180/M_PI);
    // }
    // free_data(data);
    
    // fprintf(file_math_wait, "№ Math_wait_Fs1 Math_wait_Fs2 Math_wait_Fs3 Math_wait_Omega_s1 Math_wait_Omega_s2 Math_wait_Omega_s3\n");
    // double *math_wait_fs1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_wait_fs2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_wait_fs3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_wait_omega_s1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_wait_omega_s2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_wait_omega_s3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));

    // for (size_t i = 0; i < number_no_zero_odo; i++)
    // {
    //     //printf("Iteration(math_wait) = %zu\n",i);
    //     math_wait_fs1[i] = math_wait(data.fs1,math_wait_fs1,i);
    //     math_wait_fs2[i] = math_wait(data.fs2,math_wait_fs2,i);
    //     math_wait_fs3[i] = math_wait(data.fs3,math_wait_fs3,i);
    //     math_wait_omega_s1[i] = math_wait(data.omega_s1,math_wait_omega_s1,i);
    //     math_wait_omega_s2[i] = math_wait(data.omega_s2,math_wait_omega_s2,i);
    //     math_wait_omega_s3[i] = math_wait(data.omega_s3,math_wait_omega_s3,i);
    //     fprintf(file_math_wait, " %zu %.6f %.6f %.6f %.6f %.6f %.6f\n", i, math_wait_fs1[i],math_wait_fs2[i],math_wait_fs3[i],math_wait_omega_s1[i],math_wait_omega_s2[i],math_wait_omega_s3[i]);
    // }
    // fclose(file_math_wait);
    // fprintf(file_math_dispersia, "№ Time Fs1 Fs2 Fs3 Omega_s1 Omega_s2 Omega_s3\n");
    // double *math_dispersia_s1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_dispersia_s2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_dispersia_s3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_dispersia_omega_s1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_dispersia_omega_s2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // double *math_dispersia_omega_s3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    // for (size_t i = 1; i < number_no_zero_odo; i++)
    // {
    //    // printf("Iteration(math_dispersia) = %zu\n",i);
    //     math_dispersia_s1[i] = math_dispersia(data.fs1,math_dispersia_s1,math_wait_fs1, i, number_no_zero_odo);
    //     math_dispersia_s2[i] = math_dispersia(data.fs2,math_dispersia_s2,math_wait_fs2, i, number_no_zero_odo);
    //     math_dispersia_s3[i] = math_dispersia(data.fs3,math_dispersia_s3,math_wait_fs3, i, number_no_zero_odo);
    //     math_dispersia_omega_s1[i] = math_dispersia(data.omega_s1,math_dispersia_omega_s1,math_wait_omega_s1,i,number_no_zero_odo);
    //     math_dispersia_omega_s2[i] = math_dispersia(data.omega_s2,math_dispersia_omega_s2,math_wait_omega_s2,i,number_no_zero_odo);
    //     math_dispersia_omega_s3[i] = math_dispersia(data.omega_s3,math_dispersia_omega_s3,math_wait_omega_s3,i,number_no_zero_odo);
    //     fprintf(file_math_dispersia, "%zu %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",i,data.time[i],math_dispersia_s1[i],math_dispersia_s2[i],math_dispersia_s3[i],math_dispersia_omega_s1[i],math_dispersia_omega_s2[i],math_dispersia_omega_s3[i]);
    // }
    fclose(file_math_dispersia);
    
    // free(math_wait_fs1);
    // free(math_wait_fs2);
    // free(math_wait_fs3);

    // free(math_wait_omega_s1);
    // free(math_wait_omega_s2);
    // free(math_wait_omega_s3);

    free(mean_fs1);
    free(mean_fs2);
    free(mean_fs3);

    free(mean_omega_s1);
    free(mean_omega_s2);
    free(mean_omega_s3);

    free(lat_mean);
    free_data(data);
    return 0;
}