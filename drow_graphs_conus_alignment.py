import pandas as pd
import matplotlib.pyplot as plt

# Создаем DataFrame с некоторыми данными (замените на свои данные)
file_path = r'C:\Users\Markelo\Documents\Course paper\ConusAlignment(21.02-28.02)\AlignmentOnly\ConsoleApplication1\x64\Release\Alignment\Alignment.txt'
df_conus = pd.read_csv(file_path, delimiter='    ')  # Предполагается, что данные разделены табуляциями, уточните это при необходимости

file_path1 = r'C:\Users\Markelo\Documents\Course paper\outpu_angels_1.txt'
df_alg_1 = pd.read_csv(file_path1, delimiter='    ')

print(df_alg_1[df_alg_1['Psi'] < 0]['Psi'])
plt.figure(figsize=(12, 12))
# Создаем три отдельных графика
plt.subplot(3, 2, 1)
plt.scatter(df_conus['Time'], df_conus['Heading_v[d]'])
plt.title('Heading_v[d]')

plt.subplot(3, 2, 5)
plt.scatter(df_conus['Time'], df_conus['Pitch_v[d]'])
plt.title('Pitch_v[d]')

plt.subplot(3, 2, 3)
plt.scatter(df_conus['Time'], df_conus[df_conus.columns[3]])
plt.title(df_conus.columns[3])

plt.subplot(3, 2, 6)
plt.scatter(df_alg_1['Time'], df_alg_1['Theta'])
plt.title('Theta')

plt.subplot(3, 2, 2)
plt.scatter(df_alg_1['Time'], df_alg_1['Psi'])
plt.title('Psi')

plt.subplot(3, 2, 4)
plt.scatter(df_alg_1['Time'], df_alg_1['Gamma'])
plt.title('Gamma')

plt.legend()
plt.show()
