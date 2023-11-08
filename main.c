#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

    size_t count = 0;
    double a = odo_data[0];
    for (size_t i = 0; i < len ; i++)
    {   
        count++;
        if(odo_data[i]-a!=0) break;
        a = odo_data[i];
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
    double time;     // Time[s]
    double fs1;      // Fs1[m/s/s]
    double fs2;      // Fs2[m/s/s]
    double fs3;      // Fs3[m/s/s]
    double omega_s1; // omega_s1[d/h]
    double omega_s2; // omega_s2[d/h]
    double omega_s3; // omega_s3[d/h]
    double odo;      // Odo[m]
    double lon;      // Lon[d]
    double lat;      // Lat[d]
    double hei;      // Hei[m]
    double ve;       // Ve[m/s/s]
    double vn;       // Vn[m/s/s]
    double vup;      // Vup[m/s/s]
    int flag_gps;    // FlagGPS[]
};

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

    struct Measurement *data = (struct Measurement *)malloc(num_lines * sizeof(struct Measurement));
    if (data == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }

    char line[4096];
    char *token;
    int row = 0;

    // Skip the first line with column names
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Error reading the first line");
        free(data);
        fclose(file);
        return 1;
    }
  // Read and store the data
    while (fgets(line, sizeof(line), file) != NULL) {
        token = strtok(line, " \t"); // Assuming space or tab as the delimiter

        if (token == NULL) {
            continue; // Skip empty lines
        }

        data[row].time = atof(token);
        token = strtok(NULL, " \t");
        data[row].fs1 = atof(token);
        token = strtok(NULL, " \t");
        data[row].fs2 = atof(token);
        token = strtok(NULL, " \t");
        data[row].fs3 = atof(token);
        token = strtok(NULL, " \t");
        data[row].omega_s1 = atof(token);
        token = strtok(NULL, " \t");
        data[row].omega_s2 = atof(token);
        token = strtok(NULL, " \t");
        data[row].omega_s3 = atof(token);
        token = strtok(NULL, " \t");
        data[row].odo = atof(token);
        token = strtok(NULL, " \t");
        data[row].lon = atof(token);
        token = strtok(NULL, " \t");
        data[row].lat = atof(token);
        token = strtok(NULL, " \t");
        data[row].hei = atof(token);
        token = strtok(NULL, " \t");
        data[row].ve = atof(token);
        token = strtok(NULL, " \t");
        data[row].vn = atof(token);
        token = strtok(NULL, " \t");
        data[row].vup = atof(token);
        token = strtok(NULL, " \t");
        data[row].flag_gps = atoi(token);

        row++;
    }

    fclose(file);


    // Print the first few records as an example
    for (int i = 0; i < 10; i++) {
        printf("Time: %.6f, Fs1: %.6f, Fs2: %.6f, Fs3: %.6f, omega_s1: %.6f, omega_s2: %.6f, omega_s3: %.6f, Odo: %.6f, Lon: %.6f, Lat: %.6f, Hei: %.6f, Ve: %.6f, Vn: %.6f, Vup: %.6f, FlagGPS: %d\n",
               data[i].time, data[i].fs1, data[i].fs2, data[i].fs3, data[i].omega_s1, data[i].omega_s2, data[i].omega_s3,
               data[i].odo, data[i].lon, data[i].lat, data[i].hei, data[i].ve, data[i].vn, data[i].vup, data[i].flag_gps);
    }

    double * time_data = (double *)malloc(num_lines*sizeof(double));
    double * odo_data = (double *)malloc(num_lines*sizeof(double));
    // double odo_data[num_lines];
    for(size_t i = 0; i < num_lines; i++)
    {
        time_data[i] = data[i].time; 
    };

     for(size_t i = 0; i < num_lines; i++)
    {
        odo_data[i] = data[i].odo; 
    };

    for (size_t i = 0; i < 20; i++)
    {
        printf("%.6f ", time_data[i]);
        printf("%.6f\n", odo_data[i]);
    }
    size_t number_no_zero_odo = search_first_no_zero_elements(odo_data, num_lines);
    printf("First no zero element = %d\n", number_no_zero_odo);
    printf("%.6f ", time_data[number_no_zero_odo]);
    printf("%.6f\n", odo_data[number_no_zero_odo]);
    // draw graph odo of time:
   // plotGraph(time_data, odo_data, data_length);
    // Don't forget to free the allocated memory when you're done
    free(data);

    return 0;
}

double search_mediums_sensors_mean(double * array, size_t len)
{   
    double average = 0;
    double sum = 0;
    for (size_t i = 0; i < len; i++)
    {
        sum += array[i]; 
    }
    
    return sqrt(sum)/len;
}

