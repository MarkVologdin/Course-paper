#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
//#include <cmath>

#define absolute_speed_Earth_rotation 15
# define M_PI		3.14159265358979323846
void plotGraph(double *time_data, double *odo_data, int data_length) {
    if (time_data == NULL || odo_data == NULL || data_length <= 0) {
        printf("Invalid input data.\n");
        return;
    }

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

    if (gnuplotPipe == NULL) {
        perror("Error opening gnuplot");
        return;
    }

    fprintf(gnuplotPipe, "set terminal png\n");
    fprintf(gnuplotPipe, "set output 'graph.png'\n");
    fprintf(gnuplotPipe, "set xlabel 'Time (t)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Distance (Odo)'\n");
    fprintf(gnuplotPipe, "plot '-' with linespoints title 'Odo vs. Time'\n");

    for (int i = 0; i < data_length; i++) {
        fprintf(gnuplotPipe, "%lf %lf\n", time_data[i], odo_data[i]);
    }

    fprintf(gnuplotPipe, "e\n");
    getchar();
    
    pclose(gnuplotPipe);
}

size_t search_first_no_zero_elements( double * odo_data, size_t len)
{
    double epsilon = DBL_EPSILON;
    size_t count = 0;
    double a = odo_data[0];
    printf("DBL epsilon =%.20f \n",epsilon);
    for (size_t i = 0; i < len ; i++)
    {   
        if(!fabs(odo_data[i]-a)<=epsilon*fmax(fabs(odo_data[i]),fabs(a))) break;
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
    double average = 0;
    double sum = 0;
    for (size_t i = 0; i < len; i++)
    {
        sum += array[i]; 
    }
    
    return sum/len;
}

double* search_angle_orientation_model(double * fs1, double *fs2, double * fs3, size_t len)
{
    double theta , pitch , gamma; double *angles;
    double mediums_fs1, mediums_fs2, mediums_fs3 ;

    angles = (double*)malloc(3*sizeof(double));

    mediums_fs1 = search_mediums_sensors_mean(fs1, len);
    mediums_fs2 = search_mediums_sensors_mean(fs2, len);
    mediums_fs3 = search_mediums_sensors_mean(fs3, len);

    theta = atan(mediums_fs1/(sqrt(pow(mediums_fs2,2)+pow(mediums_fs3,2))));// in radians
    gamma = -(atan(mediums_fs3/mediums_fs2)); // in radians
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

    speed_rotation = absolute_speed_Earth_rotation * cos(medium_latitude*M_PI/180);

    columns_l2[0]= (medium_omega_s1- columns_l3[0]*speed_rotation*sin(medium_latitude*M_PI/180))/(speed_rotation*cos(medium_latitude*M_PI/180));
    columns_l2[1]= (medium_omega_s2- columns_l3[1]*speed_rotation*sin(medium_latitude*M_PI/180))/(speed_rotation*cos(medium_latitude*M_PI/180));
    columns_l2[2]= (medium_omega_s3- columns_l3[2]*speed_rotation*sin(medium_latitude*M_PI/180))/(speed_rotation*cos(medium_latitude*M_PI/180));

    angles[0] = atan((columns_l2[1]*columns_l3[2]-columns_l2[2]*columns_l3[1])/(columns_l2[2]*columns_l3[0]-columns_l2[0]*columns_l3[2]));

    return angles;
}

int main() {
     char filename[] = "INS_Odo_Data.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return 1;
    }

    size_t num_lines = count_lines_in_file(file);
    printf("%d\n", num_lines);

    getchar();
    size_t size_of_struct = sizeof(struct Measurement);
    size_t total_size = size_of_struct* num_lines;
    printf("Total size data = %d\n", total_size);

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
    printf("First no zero element = %d\n", number_no_zero_odo);
    printf("%.6f ", data.time[number_no_zero_odo-2]);
    printf("%.6f\n", data.odo[number_no_zero_odo-2]);
    printf("%.6f ", data.time[number_no_zero_odo]);
    printf("%.6f\n", data.odo[number_no_zero_odo]);

    double* angles = search_angle_orientation_model(data.fs1,data.fs2,data.fs3, number_no_zero_odo); // means model orientation angles

    for (size_t i = 0; i < 3; i++)
    {
        printf("Orientation angles[%d] = %f \n",i,angles[i]*180/M_PI);
    }
    
    angles = search_angle_psi(angles, data.omega_s1, data.omega_s2, data.omega_s3, data.lat, number_no_zero_odo);

    for (size_t i = 0; i < 3; i++)
    {
        printf("Orientation angles[%d] = %f \n",i,angles[i]*180/M_PI);
    }
    // draw graph odo of time:
   // plotGraph(time_data, odo_data, data_length);
    // Don't forget to free the allocated memory when you're done
    //free(data);

    return 0;
}