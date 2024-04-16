import pandas as pd
import matplotlib.pyplot as plt
import math

RadianToDegree = 180. / math.pi


def D2R(Degrees):
    return Degrees / RadianToDegree  # Degree  --> Radians


def M2R(Minutes):
    return (Minutes / 60.) / RadianToDegree  # Minutes --> Radians


def S2R(Seconds):
    return (Seconds / 3600.) / RadianToDegree  # Seconds --> Radians


def R2D(Radians):
    return Radians * RadianToDegree  # Radians --> Degree


def R2M(Radians):
    return Radians * RadianToDegree * 60.  # Radians --> Minutes


def R2S(Radians):
    return Radians * RadianToDegree * 3600.  # Radians --> Seconds


def DH2RS(x):
    return D2R(x) / 3600.


# Создаем DataFrame с некоторыми данными (замените на свои данные)
file_path = r'C:\Users\Markelo\Documents\Course-paper\output_means.txt'
df_input = pd.read_csv(file_path,
                       delimiter=' ')  # Предполагается, что данные разделены табуляциями, уточните это при необходимости

df = df_input.iloc[::100]
plt.figure(figsize=(12, 12))

df["Omega_s1"] = DH2RS(df['Omega_s1'])
df["Omega_s2"] = DH2RS(df['Omega_s2'])
df["Omega_s3"] = DH2RS(df['Omega_s3'])
# Создаем три отдельных графика
plt.subplot(3, 2, 1)
plt.scatter(df['Time'], df['Fs1_mean'])
plt.title('Fs1_mean')

plt.subplot(3, 2, 5)
plt.scatter(df['Time'], df['Fs2_mean'])
plt.title('Fs2_mean')

plt.subplot(3, 2, 3)
plt.scatter(df['Time'], df['Fs3_mean'])
plt.title('Fs3_mean')

plt.subplot(3, 2, 6)
plt.scatter(df['Time'], df['Omega_s1'])
plt.title('Omega_s1')

plt.subplot(3, 2, 2)
plt.scatter(df['Time'], df['Omega_s2'])
plt.title('Omega_s2')

plt.subplot(3, 2, 4)
plt.scatter(df['Time'], df['Omega_s3'])
plt.title('Omega_s3')

plt.legend()
plt.show()
