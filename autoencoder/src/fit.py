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
from keras.layers import Dense, LSTM
from keras.models import Sequential, load_model
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


def get_train_set(dataset, look_back: int):
    train_size = len(dataset)
    train_x = dataset[0:train_size, :]
    train_x, train_y = create_dataset(train_x, look_back)
    return train_x, train_y


def draw_figures(original_dataset, predict1):
    plt.plot(original_dataset)
    plt.plot(predict1)
    plt.show()


if __name__ == '__main__':
    model_name = 'autoencoder.keras'
    create_model = True
    look_back = 10  # Количество предыдущих значений для оценки следующего
    data_column = 2  # Номер столбца (начиная с 0), в котором находятся данные
    file_name = 'd3c1rr4_0crb.csv'  # Название файла с данными
    epochs = 30  # Количество эпох обучения

    data = get_data_from_file(file_name, data_column)

    scaler = MinMaxScaler(feature_range=(0, 1))  # Объект для нормализации данных
    dataset = scaler.fit_transform(data.values)  # Нормализованный датасет

    train_input, train_output = get_train_set(dataset, look_back)

    # Изменяем размеры датасетов
    train_input = np.reshape(train_input, (train_input.shape[0], train_input.shape[1], 1))

    # Загружаем модель
    if create_model:
        # Создаем модель
        model = Sequential()
        model.add(LSTM(look_back + 1, input_shape=(look_back, 1)))
        model.add(Dense(3))
        model.add(Dense(1))
        model.compile(optimizer='adam', loss='mean_squared_error')
    else:
        model = load_model(model_name)

    # Тренируем модель на тренировочных данных
    model.fit(train_input, train_output, batch_size=1000, epochs=epochs, workers=6, use_multiprocessing=True)

    model.save(model_name)

    train_predict = model.predict(train_input, workers=6, use_multiprocessing=True)
    train_predict = scaler.inverse_transform(train_predict)

    draw_figures(scaler.inverse_transform(dataset), train_predict)
