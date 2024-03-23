#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define absolute_speed_Earth_rotation 15
# define M_PI		3.14159265358979323846

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
    if (data.time != NULL){free(data.time);};
    if (data.fs1 != NULL){free(data.time);};
    if (data.fs2 != NULL){free(data.time);};
    if (data.fs3 != NULL){free(data.time);};
    if (data.omega_s1 != NULL){free(data.time);};
    if (data.omega_s2 != NULL){free(data.time);};
    if (data.omega_s3 != NULL){free(data.time);};
    if (data.odo != NULL){free(data.time);};
    if (data.hei != NULL){free(data.time);};
    if (data.lon != NULL){free(data.time);};
    if (data.lat != NULL){free(data.time);};
    if (data.hei != NULL){free(data.time);};
    if (data.ve != NULL){free(data.time);};
    if (data.vn != NULL){free(data.time);};
    if (data.vup != NULL){free(data.time);};
    if (data.flag_gps != NULL){free(data.time);};


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
double* search_angle_orientation_model_1(double * fs1, double *fs2, double * fs3, size_t len)
{
    double theta , gamma; double *angles;
    double mediums_fs1, mediums_fs2, mediums_fs3 ; double * mediums_seans;

    angles = (double*)malloc(3*sizeof(double));
    mediums_seans = (double*)malloc(3*sizeof(double));

    mediums_fs1 = search_mediums_sensors_mean(fs1, len);
    mediums_fs2 = search_mediums_sensors_mean(fs2, len);
    mediums_fs3 = search_mediums_sensors_mean(fs3, len);

    mediums_seans[0] = mediums_fs1;
    mediums_seans[1] = mediums_fs2;
    mediums_seans[2] = mediums_fs3;

    double f1_norm,f2_norm, f3_norm;

    f1_norm = mediums_fs1/norma_vectora(mediums_seans);
    f2_norm = mediums_fs2/norma_vectora(mediums_seans);
    f3_norm = mediums_fs3/norma_vectora(mediums_seans);

    theta = atan2(f1_norm,(sqrt(pow(f2_norm,2)+pow(f3_norm,2))));// in radians
    gamma = -(atan2(f3_norm,f2_norm)); // in radians
    angles[0] = 0;
    angles[1] = gamma;
    angles[2] = theta; 

    return angles;
}
double* search_angle_orientation_model_2_ortoganalization(double * fs1, double *fs2, double * fs3, size_t len)
{
    double theta , gamma; double *angles;
    double mediums_fs1, mediums_fs2, mediums_fs3 ;

    angles = (double*)malloc(3*sizeof(double));

    mediums_fs1 = search_mediums_sensors_mean(fs1, len);
    mediums_fs2 = search_mediums_sensors_mean(fs2, len);
    mediums_fs3 = search_mediums_sensors_mean(fs3, len);

    theta = atan2(mediums_fs1,(sqrt(pow(mediums_fs2,2)+pow(mediums_fs3,2))));// in radians
    gamma = -(atan2(mediums_fs3,mediums_fs2)); // in radians
    angles[0] = 0;
    angles[1] = gamma;
    angles[2] = theta; 

    return angles;
}
double * search_angle_psi(double * angles, double *omega_s1,double *omega_s2,double *omega_s3, double* lat, size_t len)
{
    double medium_omega_s1, medium_omega_s2, medium_omega_s3,  medium_latitude, speed_rotation;
    double* columns_l2; double* columns_l3;

    columns_l3 = (double*)malloc(3*sizeof(double));
    columns_l2 = (double*)malloc(3*sizeof(double));

    columns_l3[0] = sin(angles[2]); // sin(radians), cos(radians)
    columns_l3[1] = cos(angles[2])*cos(angles[1]);
    columns_l3[2] = -cos(angles[2])*sin(angles[1]);

    medium_omega_s1 = search_mediums_sensors_mean(omega_s1, len);
    medium_omega_s2 = search_mediums_sensors_mean(omega_s2, len);
    medium_omega_s3 = search_mediums_sensors_mean(omega_s3, len);

    medium_latitude = search_mediums_sensors_mean(lat, len);

    speed_rotation = absolute_speed_Earth_rotation;//absolute_speed_Earth_rotation * cos(medium_latitude*M_PI/180);

    columns_l2[0]= (medium_omega_s1- columns_l3[0]*speed_rotation*sin(medium_latitude*M_PI/180))/(speed_rotation*cos(medium_latitude*M_PI/180));
    columns_l2[1]= (medium_omega_s2- columns_l3[1]*speed_rotation*sin(medium_latitude*M_PI/180))/(speed_rotation*cos(medium_latitude*M_PI/180));
    columns_l2[2]= (medium_omega_s3- columns_l3[2]*speed_rotation*sin(medium_latitude*M_PI/180))/(speed_rotation*cos(medium_latitude*M_PI/180));

    angles[0] = atan2((columns_l2[1]*columns_l3[2]-columns_l2[2]*columns_l3[1]),(columns_l2[2]*columns_l3[0]-columns_l2[0]*columns_l3[2]));

    return angles;
}
double * search_angle_psi_2(double *f_medium, double * angles, double *omega_s1,double *omega_s2,double *omega_s3, double *columns_l3, size_t len)
{
    double medium_omega_s1, medium_omega_s2, medium_omega_s3;
    //double* columns_l2; 
    double* columns_l1;
    columns_l3[0] = columns_l3[0]*1;
    columns_l1 = (double*)malloc(3*sizeof(double));
    //columns_l2 = (double*)malloc(3*sizeof(double));

    medium_omega_s1 = search_mediums_sensors_mean(omega_s1, len);
    medium_omega_s2 = search_mediums_sensors_mean(omega_s2, len);
    medium_omega_s3 = search_mediums_sensors_mean(omega_s3, len);

    double * medium_omega = (double*)malloc(3*sizeof(double));

    medium_omega[0] = medium_omega_s1;
    medium_omega[1] = medium_omega_s2;
    medium_omega[2] = medium_omega_s3;

    for (size_t i = 0; i < 3; i++)
    {
        columns_l1[i] = (vector_product(medium_omega, f_medium, 3)[i])/norma_vectora(vector_product(medium_omega, f_medium, 3));
    }

   // columns_l2 = vector_product(columns_l3, columns_l1,3);

    angles[0] = atan2((columns_l1[0]),(columns_l1[1]));

    return angles;
}

int main() {
    char filename[] = "INS_Odo_Data.txt";
    FILE *file = fopen(filename, "r");
    FILE *Ffile1 = fopen("outpu_angels_1.txt", "w+");
    FILE *Ffile2 = fopen("outpu_angels_2.txt", "w+");
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
    // printf("%I64u\n", num_lines);

    // getchar();
    size_t size_of_struct = sizeof(struct Measurement);
    size_t total_size = size_of_struct* num_lines;
    printf("Total size data = %I64u\n", total_size);

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
        data.fs1[row] = atof(token);
        token = strtok(NULL, " \t");
        data.fs2[row] = atof(token);
        token = strtok(NULL, " \t");
        data.fs3[row] = atof(token);
        token = strtok(NULL, " \t");
        data.omega_s1[row] = atof(token);
        token = strtok(NULL, " \t");
        data.omega_s2[row] = atof(token);
        token = strtok(NULL, " \t");
        data.omega_s3[row] = atof(token);
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

    fclose(file);


    // // Print the first few records as an example
    // for (int i = 0; i < 10; i++) {
    //     printf("Time: %.6f, Fs1: %.6f, Fs2: %.6f, Fs3: %.6f, omega_s1: %.6f, omega_s2: %.6f, omega_s3: %.6f, Odo: %.6f, Lon: %.6f, Lat: %.6f, Hei: %.6f, Ve: %.6f, Vn: %.6f, Vup: %.6f, FlagGPS: %d\n",
    //            data[i].time, data[i].fs1, data[i].fs2, data[i].fs3, data[i].omega_s1, data[i].omega_s2, data[i].omega_s3,
    //            data[i].odo, data[i].lon, data[i].lat, data[i].hei, data[i].ve, data[i].vn, data[i].vup, data[i].flag_gps);
    // }

    
    size_t number_no_zero_odo = search_first_no_zero_elements(data.odo, num_lines);
    printf("First no zero element = %I64u\n", number_no_zero_odo);
    printf("%.6f ", data.time[number_no_zero_odo-2]);
    printf("%.6f\n", data.odo[number_no_zero_odo-2]);
    printf("%.6f ", data.time[number_no_zero_odo]);
    printf("%.6f\n", data.odo[number_no_zero_odo]);

    size_t number_of_steps = number_no_zero_odo/600;

    double* angles1; double* angles2; 
    double * f_medium = (double*)malloc(3*sizeof(double)); double * columns_l3;

    columns_l3 = (double*)malloc(3*sizeof(double));

    fprintf(Ffile1, "№ Time Psi Gamma Theta\n");
    fprintf(Ffile2, "№ Time Psi Gamma Theta\n");
    printf("Number of steps=%I64u,number no zero odo = %I64u\n", number_of_steps, number_no_zero_odo);
    for (size_t i = number_of_steps*100; i < number_no_zero_odo; i+=number_of_steps*10)
    {
        f_medium[0] = search_mediums_sensors_mean(data.fs1, i);
        f_medium[1] = search_mediums_sensors_mean(data.fs2, i);
        f_medium[2] = search_mediums_sensors_mean(data.fs3, i);

        columns_l3[0] = f_medium[0]/norma_vectora(f_medium);
        columns_l3[1] = f_medium[1]/norma_vectora(f_medium);
        columns_l3[2] = f_medium[2]/norma_vectora(f_medium);

        printf("Iteration = %I64u\n", i);
        angles1 = search_angle_orientation_model_1(data.fs1,data.fs2,data.fs3, i); // means model orientation angles
        angles1 = search_angle_psi(angles1, data.omega_s1, data.omega_s2, data.omega_s3, data.lat, i);
        angles2 = search_angle_orientation_model_2_ortoganalization(data.fs1,data.fs2,data.fs3, i); // means model orientation angles
        angles2 = search_angle_psi_2(f_medium,angles2, data.omega_s1, data.omega_s2, data.omega_s3,columns_l3, i);
        printf("Version1 => %.6f %.6f %.6f\n", angles1[0],angles1[1],angles1[2]);
        printf("Version2 => %.6f %.6f %.6f\n", angles2[0],angles2[1],angles2[2]);

        //fprintf(file_math, " %I64u %.6f %.6f %.6f %.6f %.6f %.6f\n", i, math_wait(data.fs1, i/number_of_steps),math_wait(data.fs2, i), math_wait(data.fs3, i), math_wait(data.omega_s1, i), math_wait(data.omega_s2, i), math_wait(data.omega_s3, i));
        fprintf(Ffile1, "%I64u %.6f %.6f %.6f %.6f \n", i, data.time[i], angles1[0]*180/M_PI, angles1[1]*180/M_PI, angles1[2]*180/M_PI);
        fprintf(Ffile2, "%I64u %.6f %.6f %.6f %.6f \n", i, data.time[i], angles2[0]*180/M_PI, angles2[1]*180/M_PI, angles2[2]*180/M_PI);
    }
    fclose(Ffile1);
    fclose(Ffile2);
    // for (size_t i = 0; i < 3; i++)
    // {
    //     printf("Orientation angles[%I64u] = %f \n",i,angles[i]*180/M_PI);
    // }
    
    // for (size_t i = 0; i < 3; i++)
    // {
    //     printf("Orientation version1 angles[%I64u] = %f \n",i,angles1[i]*180/M_PI);
    //     printf("Orientation version2 angles[%I64u] = %f \n",i,angles2[i]*180/M_PI);
    // }
    // free_data(data);
    
    fprintf(file_math_wait, "№ Math_wait_Fs1 Math_wait_Fs2 Math_wait_Fs3 Math_wait_Omega_s1 Math_wait_Omega_s2 Math_wait_Omega_s3\n");
    double *math_wait_fs1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_wait_fs2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_wait_fs3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_wait_omega_s1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_wait_omega_s2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_wait_omega_s3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));

    for (size_t i = 0; i < number_no_zero_odo; i++)
    {
        //printf("Iteration(math_wait) = %I64u\n",i);
        math_wait_fs1[i] = math_wait(data.fs1,math_wait_fs1,i);
        math_wait_fs2[i] = math_wait(data.fs2,math_wait_fs2,i);
        math_wait_fs3[i] = math_wait(data.fs3,math_wait_fs3,i);
        math_wait_omega_s1[i] = math_wait(data.omega_s1,math_wait_omega_s1,i);
        math_wait_omega_s2[i] = math_wait(data.omega_s2,math_wait_omega_s2,i);
        math_wait_omega_s3[i] = math_wait(data.omega_s3,math_wait_omega_s3,i);
        fprintf(file_math_wait, " %I64u %.6f %.6f %.6f %.6f %.6f %.6f\n", i, math_wait_fs1[i],math_wait_fs2[i],math_wait_fs3[i],math_wait_omega_s1[i],math_wait_omega_s2[i],math_wait_omega_s3[i]);
    }
    fclose(file_math_wait);
    fprintf(file_math_dispersia, "№ Time Fs1 Fs2 Fs3 Omega_s1 Omega_s2 Omega_s3\n");
    double *math_dispersia_s1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_dispersia_s2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_dispersia_s3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_dispersia_omega_s1 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_dispersia_omega_s2 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    double *math_dispersia_omega_s3 = (double*)malloc(sizeof(double)*(number_no_zero_odo+1));
    for (size_t i = 1; i < number_no_zero_odo; i++)
    {
       // printf("Iteration(math_dispersia) = %I64u\n",i);
        math_dispersia_s1[i] = math_dispersia(data.fs1,math_dispersia_s1,math_wait_fs1, i, number_no_zero_odo);
        math_dispersia_s2[i] = math_dispersia(data.fs2,math_dispersia_s2,math_wait_fs2, i, number_no_zero_odo);
        math_dispersia_s3[i] = math_dispersia(data.fs3,math_dispersia_s3,math_wait_fs3, i, number_no_zero_odo);
        math_dispersia_omega_s1[i] = math_dispersia(data.omega_s1,math_dispersia_omega_s1,math_wait_omega_s1,i,number_no_zero_odo);
        math_dispersia_omega_s2[i] = math_dispersia(data.omega_s2,math_dispersia_omega_s2,math_wait_omega_s2,i,number_no_zero_odo);
        math_dispersia_omega_s3[i] = math_dispersia(data.omega_s3,math_dispersia_omega_s3,math_wait_omega_s3,i,number_no_zero_odo);
        fprintf(file_math_dispersia, "%I64u %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",i,data.time[i],math_dispersia_s1[i],math_dispersia_s2[i],math_dispersia_s3[i],math_dispersia_omega_s1[i],math_dispersia_omega_s2[i],math_dispersia_omega_s3[i]);
    }
    fclose(file_math_dispersia);
    
    free(math_wait_fs1);
    free(math_wait_fs2);
    free(math_wait_fs3);
    free(math_wait_omega_s1);
    free(math_wait_omega_s2);
    free(math_wait_omega_s3);
    
    return 0;
}