# ----------------------------------------------------------------------
# Значит так, программа работает следующим образом:
# 1 Парсим данные и кидаем их в отдельный файл:
#   1.1 Можно в файл кидать только значения датчика, остальное не нужно, но можно))
# 2 Пишем путь к файлу в переменную file_name
# 3 Уже можно запускать код и ждать ответ
# 4 Для настройки есть параметры:
#   4.1 epochs - параметр отвечает за количество эпох обучения,
#       если видно, что в консоли значение loss приближается к нулю быстрее, чем проходят все эпохи,
#       то можно сделать его меньше
#   4.2 data_column - номер столбца в файле, где находятся данные, менять, если данные не в 3 столбце
#   4.3 look_back - с этим параметром можно поиграть и попробовать подобрать оптимальный
#   4.4 percent_of_train_data - процент данных от всего набора для обучения (0.6 значит,
#       что 60% от всех данных будут использованы для обучения нейронки, а остальные для проверки)
# Ну вот и все! Пользуйся!
# ----------------------------------------------------------------------
import keras
from keras.models import load_model
from pandas import read_csv
from sklearn.preprocessing import MinMaxScaler
import matplotlib.pyplot as plt
import numpy as np


def create_dataset(dataset, look_back=1):
    data_x, data_y = [], []
    for i in range(len(dataset) - look_back - 1):
        a = dataset[i:(i + look_back), 0]
        data_x.append(a)
        data_y.append(dataset[i + look_back, 0])
    return np.array(data_x), np.array(data_y)


def get_data_from_file(file_name: str, data_column: int):
    return read_csv(file_name, sep=';', header=None, usecols=[data_column], engine='python')


def get_predict_set(dataset, look_back: int):
    size = len(dataset)
    predict_x = dataset[0:size, :]
    predict_x, predict_y = create_dataset(predict_x, look_back)
    return predict_x, predict_y


def draw_figures(time, original_dataset, predict1, look_back):
    plt.plot(time, original_dataset)
    plt.plot(time[look_back+1:], predict1)
    plt.show()


if __name__ == '__main__':
    model_name = 'autoencoder.keras'
    sid = 1
    look_back = 10  # Количество предыдущих значений для оценки следующего
    data_column = 2  # Номер столбца (начиная с 0), в котором находятся данные
    file_name = 'd3c1rr4_0crb.csv'  # Название файла с данными

    time = get_data_from_file(file_name, 1)
    data = get_data_from_file(file_name, data_column)

    scaler = MinMaxScaler(feature_range=(0, 1))  # Объект для нормализации данных
    dataset = scaler.fit_transform(data.values)  # Нормализованный датасет

    predict_input, train_output = get_predict_set(dataset, look_back)

    # Изменяем размеры датасетов
    predict_input = np.reshape(predict_input, (predict_input.shape[0], predict_input.shape[1], 1))

    model: keras.Model = load_model(model_name)

    # Получаем предсказания нейросети для данных
    predict = model.predict(predict_input, workers=6, use_multiprocessing=True)

    # Возвращаем данные в исходный вид, как до нормализации
    predict_data = scaler.inverse_transform(predict)
    real_data = scaler.inverse_transform(dataset)

    with open('predicted_' + file_name, 'w') as file:
        for real_value, value in zip(range(look_back + 1, len(data.values)), predict_data):
            if not value - 5 <= data.values[real_value] <= value + 5:
                print(f'!!!WARNING!!! time: {time.values[real_value][0]}\t '
                      f'real: {data.values[real_value][0]}\t predict: {value[0]}')
            file.write(f'{sid};{time.values[real_value][0]};{value[0]}\n')

    draw_figures(time, real_data, predict_data, look_back)
