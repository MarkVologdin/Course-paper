import matplotlib.pyplot as plt
import pandas as pd

# Загрузка данных из файла
file_path = '/home/mark/Документы/Course paper/output_math_wait'  # Укажите путь к вашему файлу данных
data = pd.read_csv(file_path, delim_whitespace=True)

# Создание отдельных графиков
plt.figure(figsize=(10, 6))

# График 1
plt.plot(data['№'], data['Math_wait_Fs1'])
plt.title('Math_wait_Fs1 vs Record Number')
plt.xlabel('Record Number')
plt.ylabel('Math_wait_Fs1')
plt.show()

# График 2
plt.figure(figsize=(10, 6))
plt.plot(data['№'], data['Math_wait_Fs2'])
plt.title('Math_wait_Fs2 vs Record Number')
plt.xlabel('Record Number')
plt.ylabel('Math_wait_Fs2')
plt.show()

# График 3
plt.figure(figsize=(10, 6))
plt.plot(data['№'], data['Math_wait_Fs3'])
plt.title('Math_wait_Fs3 vs Record Number')
plt.xlabel('Record Number')
plt.ylabel('Math_wait_Fs3')
plt.show()

# График 4
plt.figure(figsize=(10, 6))
plt.plot(data['№'], data['Math_wait_Omega_s1'])
plt.title('Math_wait_Omega_s1 vs Record Number')
plt.xlabel('Record Number')
plt.ylabel('Math_wait_Omega_s1')
plt.show()

# График 5
plt.figure(figsize=(10, 6))
plt.plot(data['№'], data['Math_wait_Omega_s2'])
plt.title('Math_wait_Omega_s2 vs Record Number')
plt.xlabel('Record Number')
plt.ylabel('Math_wait_Omega_s2')
plt.show()

# График 6
plt.figure(figsize=(10, 6))
plt.plot(data['№'], data['Math_wait_Omega_s3'])
plt.title('Math_wait_Omega_s3 vs Record Number')
plt.xlabel('Record Number')
plt.ylabel('Math_wait_Omega_s3')
plt.show()
