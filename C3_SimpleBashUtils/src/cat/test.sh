#!/bin/bash

echo "------------------1------------------"
echo "Команда s21_cat -b 1.txt"
./s21_cat -b 1.txt
echo "-------------------------------------"
echo "Стандартная команда cat -b 1.txt"
cat -b 1.txt

echo "------------------2------------------"
echo "Команда s21_cat -e 1.txt"
./s21_cat -e 1.txt
echo "-------------------------------------"
echo "Стандартная команда cat -e 1.txt"
cat -e 1.txt

echo "------------------3------------------"
echo "Команда s21_cat -n 1.txt"
./s21_cat -n 1.txt
echo "-------------------------------------"
echo "Стандартная команда cat -n 1.txt"
cat -n 1.txt

echo "------------------4------------------"
echo "Команда s21_cat -s 1.txt"
./s21_cat -s 1.txt
echo "-------------------------------------"
echo "Стандартная команда cat -s 1.txt"
cat -s 1.txt

echo "------------------5------------------"
echo "Команда s21_cat -t 1.txt"
./s21_cat -t 1.txt
echo "-------------------------------------"
echo "Стандартная команда cat -t 1.txt"
cat -t 1.txt

echo "-------------------------------------"
echo "Конец :)"

