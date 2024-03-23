import matplotlib.pyplot as plt
import pandas as pd

# Загрузка данных из файла
file_path = '/home/mark/Документы/Course paper/output_math_wait'  # Укажите путь к вашему файлу данных
data = pd.read_csv(file_path, delim_whitespace=True)

# Создание графика
plt.figure(figsize=(10, 6))

# График Fs1
plt.plot(data['№'], data['Math_wait_Omega_s1'], label='Omega_s1', linestyle='-', marker='o')

# График Fs2
plt.plot(data['№'], data['Math_wait_Omega_s2'], label='Omega_s2', linestyle='--', marker='s')

# График Fs3
plt.plot(data['№'], data['Math_wait_Omega_s3'], label='Omega_s3', linestyle=':', marker='^')

# Добавление подписей к осям и легенды
plt.title('Comparison of Math_wait Omega_s1, Omega_s2, and Omega_s3')
plt.xlabel('Record Number')
plt.ylabel('Values')
plt.legend()

# Показать график
plt.show()
