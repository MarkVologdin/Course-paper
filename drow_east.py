import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Создаем DataFrame с некоторыми данными (замените на свои данные)
file_path = r'C:\Users\Markelo\Documents\Course paper\ConusAlignment(21.02-28.02)\AlignmentOnly\ConsoleApplication1\x64\Release\Alignment\Alignment.txt'
df_conus = pd.read_csv(file_path, delimiter='    ')  # Предполагается, что данные разделены табуляциями, уточните это при необходимости

file_path1 = r'C:\Users\Markelo\Documents\Course paper\outpu_angels_1.txt'
df_alg_1 = pd.read_csv(file_path1, delimiter='    ')

print(df_alg_1.head(10))
# Создаем три отдельных графика
plt.subplot(1,1,1)
plt.scatter(df_alg_1['Time'], df_alg_1['Gamma'])
plt.yticks(np.arange(min(df_alg_1['Gamma']),max(df_alg_1['Gamma'])))
plt.show()