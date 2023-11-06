#!/bin/bash

# Массив флагов
flags=("" "-b" "-e" "-n" "-s" "-t" "-v")

# Массив файлов
files=("1.txt" "2.txt" "3.txt" "4.txt" "5.txt" "6.txt")

# Инициализация счетчиков
success=0
fail=0
all=0

# Функция для тестирования cat_print с заданными флагами и файлами
test_cat_print() {
  local flags="$1"
  shift
  local files=("$@")

  # Запуск cat_print с заданными флагами и файлами
  ./s21_cat $flags "${files[@]}"
}

# Проходим по всем комбинациям флагов и файлов
for flag in "${flags[@]}"; do
  for file in "${files[@]}"; do
    all=$((all + 1))
    echo "ТЕСТ: Флаг: $flag, Файл: $file"

    # Запуск встроенной cat
    cat_result=$(cat $flag "$file")

    # Запуск s21_cat
    s21_cat_result=$(test_cat_print "$flag" "$file")

    # Сравниваем результаты
    if [ "$cat_result" == "$s21_cat_result" ]; then
    #   echo "совпали"
      success=$((success + 1))
    else
      echo "НЕ СОВПАЛИ!"
      echo
    #   echo "МОЙ:"
    #   echo $s21_cat_result
    #   echo "ОРИГ:"
    #   echo $cat_result
      fail=$((fail + 1))
    fi
  done
done

# Вывод общего результата
echo "Успешных тестов: $success/$all"
echo "Неуспешных тестов: $fail/$all"
