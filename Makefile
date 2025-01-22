CC = gcc               # Компилятор
CFLAGS = -Wall -g      # Флаги для компиляции (например, включение отладочной информации)
TARGET = shell         # Имя исполнимого файла
OBJ = shell.o game.o   # Объектные файлы, которые нужно скомпилировать

# Правило для компиляции игры
game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c -o game.o

# Правило для компиляции оболочки
shell.o: shell.c game.h
	$(CC) $(CFLAGS) -c shell.c -o shell.o

# Правило для связывания объектных файлов в исполнимый файл
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Очистка объектных файлов и исполнимого файла
clean:
	rm -f $(OBJ) $(TARGET)
