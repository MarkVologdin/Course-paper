# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import pandas as pd

# Загрузка данных из файла
file_path1 = 'C:\\Users\\Markelo\\Documents\\Course paper\\outpu_angels_1.txt'
file_path2 = 'C:\\Users\\Markelo\\Documents\\Course paper\\outpu_angels_2.txt'

data1 = pd.read_csv(file_path1, delim_whitespace=True)
data2 = pd.read_csv(file_path2, delim_whitespace=True)


## Создание графика
plt.figure(figsize=(10, 6))
data1['Psi'] = data1['Psi'] - 90
data2['Psi'] = data2['Psi'] - 90
# График
plt.plot(data1['Time'], data1['Psi'], label='Psi', linestyle='-', marker='o')

# График 
plt.plot(data2['Time'], data2['Psi'], label='Psi', linestyle='--', marker='s')


# Добавление подписей к осям и легенды
plt.title('Comparison of Psi1 and Psi2')
plt.xlabel('TIme')
plt.ylabel('Values')
plt.legend()

# Показать график
plt.show()