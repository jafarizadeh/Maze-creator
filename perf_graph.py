import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

file_path = 'performance.log' 
data = pd.read_csv(file_path, delim_whitespace=True, header=0)

data.columns = ['Taille', 'ExecutionTime',
                'Memoire']  


def extract_area(taille):
    largeur, hauteur = map(int, taille.split('x'))
    return largeur * hauteur 


data['Aire'] = data['Taille'].apply(extract_area)

print(data)

plt.figure(figsize=(10, 6))
plt.plot(data['Aire'],
         data['ExecutionTime'],
         marker='o',
         color='g',
         label='Execution Time')
plt.xlabel('Maze Dimensions (Width x Height)')
plt.ylabel('Execution Time (seconds)')
plt.title('Graph of Execution Time and Maze Dimensions')
plt.legend()
plt.grid()
plt.show()
