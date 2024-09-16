#include <algorithm>
#include <iostream>
#include <string>

// Класс для работы с большими числами
class BigInteger
{
private:
    std::string str;
public:
    BigInteger() = delete;
    BigInteger(std::string & st);
    BigInteger(const BigInteger & other);
    BigInteger(const BigInteger && other);
    BigInteger & operator=(const BigInteger & other);
    BigInteger & operator=(const BigInteger && other);
    ~BigInteger();
    BigInteger & add(BigInteger & other);
    BigInteger & subtract(BigInteger & other);
    BigInteger & multiply(BigInteger & fir, BigInteger & sec);
    BigInteger & divide(BigInteger & other);
    BigInteger & operator+(BigInteger & other);
    BigInteger & operator-(BigInteger & other);
    BigInteger & operator*(BigInteger & other);
    BigInteger & operator/(BigInteger & other);
    std::string & getResult();
    void setResult(std::string & str);
    void setResult(std::string && str);
};

BigInteger::BigInteger(std::string & st)
{
    str = st;
}

BigInteger::BigInteger(const BigInteger & other)
{
    if (this != &other)
	*this = other;
}

BigInteger::BigInteger(const BigInteger && other)
{
    if (this != &other)
	*this = std::move(other);
}

BigInteger & BigInteger::operator=(const BigInteger & other)
{
    if (this != &other)
	str = other.str;
    
    return *this;
}

BigInteger & BigInteger::operator=(const BigInteger && other)
{
    if (this != &other)
	str = std::move(other.str);
    
    return *this;
}

BigInteger::~BigInteger() {}

BigInteger & BigInteger::operator+(BigInteger & other)
{
    this->add(other);
    return *this;
}

BigInteger & BigInteger::operator-(BigInteger & other)
{
    this->subtract(other);
    return *this;
}

BigInteger & BigInteger::operator*(BigInteger & other)
{
    this->multiply(*this, other);
    return *this;
}

BigInteger & BigInteger::operator/(BigInteger & other)
{
    this->divide(other);
    return *this;
}

std::string & BigInteger::getResult()
{
    return this->str;
}

void BigInteger::setResult(std::string & str)
{
    this->str = str;
}

void BigInteger::setResult(std::string && str)
{
    this->str = std::move(str);
}

// Операция сложения для двух длинных чисел (алгоритм сложения в
// столбик, вычислительная сложность O(n))
BigInteger & BigInteger::add(BigInteger & other) {

    // Создаем переменные для хранения длин двух строк, переменную для хранения
    // наибольшей из двух длины строки, переменные суммы двух цифр в одном
    // столбце, запоминаемого значения, строку ведущих нулей и результата вычитания
    std::string &first = this->getResult();
    std::string &second = other.getResult();
    int first_size = first.size();
    int second_size = second.size();  
    int max_size = 0;
    int column_summ = 0;
    int carry = 0;
    std::string leading_zeroes = ""; 
    std::string result = "";

    // Вычисляем строку наибольшей длины
    if (first_size > second_size)
	max_size = first_size;
    else
	max_size = second_size;
    
    // Дополняем самую короткую из двух строк строку нулями для возможности
    // дальнейшего вычитания этих строк
    if (first_size < max_size)
    {
	for (int i = 0; i < max_size - first_size; i++)
	    leading_zeroes += "0";
	
	first.insert(0, leading_zeroes);
    }

    if (second_size < max_size)
    {
	for (int j = 0; j < max_size - second_size; j++)
	    leading_zeroes += "0";
	
	second.insert(0, leading_zeroes);
    }
            
    // Заполняем результирующую строку справа-налево как при сложении в
    // столбик
    for (int i = max_size - 1; i >= 0; i--)
    { 
	// Вычитаем код ASCII символа '0' из кода ASCII каждого из двух
	// символов, находящихся в текущей колонке чтобы получить корректное
	// целочисленное значение двух искомых цифр, а не коды ASCII этих цифр,
	// затем складываем две эти цифры и добавляем запоминаемое значение к
	// сумме колонки
	column_summ = first[i] - '0' + second[i] - '0' + carry;

	// Запоминаемое значение - это первая цифра суммы колонки (если сумма
	// колонки больше 9, если же она меньше 10 - запоминаемое значение в
	// результате деления округляется до нуля)
	carry = column_summ / 10;

	// Результат суммирования колонки добавляем в самое начало строки
	result.insert(0, std::to_string(column_summ % 10));
    }

    // Если запоминаемое значение после всех суммирований колонок осталось
    // равным единице - значит сумма предыдущей колонки была больше 9, остается
    // дописать это запоминаемое значение в начало строки
    if (carry == 1)
	result.insert(0, std::to_string(carry));

    this->setResult(result);
    
    // Возвращаем результирующую строку
    return *this;
}

// Операция вычитания для двух длинных чисел (алгоритм вычитания в
// столбик, вычислительная сложность O(n))
BigInteger & BigInteger::subtract(BigInteger & other) {
    
    // Создаем переменные для хранения длин двух строк, переменную для хранения
    // наибольшей из двух длины строки, переменные разности двух цифр в одном
    // столбце, запоминаемого значения, строку ведущих нулей и результата вычитания
    std::string &first = this->getResult();
    std::string &second = other.getResult();
    int first_size = first.size();
    int second_size = second.size();  
    int max_size = 0;
    int column_diff = 0;
    int carry = 0;
    int flag = 0;
    std::string leading_zeroes = ""; 
    std::string result = "";

    if (first == second)
    {
	this->setResult("0");
	return *this;
    }

    // Вычисляем строку наибольшей длины
    if (first_size > second_size)
	max_size = first_size;
    else
	max_size = second_size;

    // Дополняем самую короткую из двух строк строку нулями для возможности
    // дальнейшего вычитания этих строк
    if (first_size < max_size)
    {
	for (int i = 0; i < max_size - first_size; i++)
	    leading_zeroes += "0";
	
	first.insert(0, leading_zeroes);
    }

    if (second_size < max_size)
    {
	for (int j = 0; j < max_size - second_size; j++)
	    leading_zeroes += "0";
	
	second.insert(0, leading_zeroes);
    }
    
    // Заполняем результирующую строку справа-налево как при вычитании в
    // столбик
    for (int i = max_size - 1; i >= 0; i--) {
	
	// Вычитаем код ASCII символа '0' из кода ASCII каждого из двух
	// символов, находящихся в текущей колонке чтобы получить корректное
	// целочисленное значение двух искомых цифр, а не коды ASCII этих цифр,
	// затем проверяем, чтобы разность всех чисел вместе с запоминаемым
	// значением была больше или равна нулю - если это так, то это значение
	// и есть значение колонки и запоминаемых значений нет; если разность
	// получилась меньше нуля - для первого числа занимаем разряд десятков
	// и запоминаем единицу
	int first_value = first[i] - '0';
	int second_value = second[i] - '0';
	int difference = first_value - carry - second_value;

	if (difference >= 0)
	{
	    column_diff = difference;
	    carry = 0;
	}
	else
	{
	    column_diff = 10 + difference;
	    carry = 1;
	}

	// Если число отрицательно - производим вычитание из правого числа левого с
	// другим знаком
	if (i == 0 && difference < 0)
	{
	    BigInteger integer = other;
	    std::string s = (integer - *this).getResult();
	    this->setResult("-" + s );
	    
	    return *this;
	}

	// Результат вычитания двух цифр в колонке добавляем в самое
	// начало строки
        result.insert(0, std::to_string(column_diff));
    }

    this->setResult(result.erase(0, result.find_first_not_of('0')));


    // Возвращаем результирующую строку со стёртыми ведущими нулями
    return *this;
}

// Операция умножения для двух длинных чисел (умножение Карацубы за
// время O(n^log3(n))
BigInteger & BigInteger::multiply(BigInteger & fir, BigInteger & sec) {
    
    // Создаем переменные для хранения длин двух строк, переменную для хранения
    // наибольшей из двух длины строки, строку ведущих нулей, младших значащих
    // нулей и результата умножения
    std::string &first = fir.getResult();
    std::string &second = sec.getResult();
    int first_size = first.size();
    int second_size = second.size();
    int max_size = 0;
    std::string leading_zeroes = "";
    std::string leasts_significant = "";
    std::string result = "";
    
    // Вычисляем строку наибольшей длины
    if (first_size > second_size)
	max_size = first_size;
    else
	max_size = second_size;

    // Дополняем самую короткую из двух строк строку нулями для возможности
    // дальнейшего вычитания этих строк
    if (first_size < max_size)
    {
	for (int i = 0; i < max_size - first_size; i++)
	    leading_zeroes += "0";
	
	first.insert(0, leading_zeroes);
    }

    if (second_size < max_size)
    {
	for (int j = 0; j < max_size - second_size; j++)
	    leading_zeroes += "0";
	
	second.insert(0, leading_zeroes);
    }

    // Базовый случай: длина наибольшей строки стала равной единице,
    // перемножаем две цифры из первой и второй строки первого столбца
    // и возвращаем результат
    if (max_size == 1)
    {
	result = std::to_string((first[0] - '0') * (second[0] - '0'));
	this->setResult(result);
        return *this;
    }

    // Разбиваем исходные задачи на подзадачи длины n/2^k
    std::string first_left_side = first.substr(0, max_size / 2);
    std::string first_right_side = first.substr(max_size / 2, max_size - max_size / 2);
    std::string second_left_side = second.substr(0, max_size / 2);
    std::string second_right_side = second.substr(max_size / 2, max_size - max_size / 2);

    BigInteger bigint1(first_left_side), bigint2(second_left_side), bigint3(first_right_side), bigint4(second_right_side);

    // Вычисляем три произведения для подзадач
    std::string P1 = multiply(bigint1, bigint2).getResult();
    std::string P2 = multiply(bigint3, bigint4).getResult();
    std::string P3 = multiply(bigint1 + bigint3, bigint2 + bigint4).getResult();

    bigint1.setResult(P1);
    bigint2.setResult(P2);
    bigint3.setResult(P3);

    // Считаем P3 - (P1 + P2) = P3 - P1 - P2
    std::string P1_3 = (bigint3 - bigint1 - bigint2).getResult();
    bigint4.setResult(P1_3);

    // Для сдвига разрядов влево (P1 * 2^2*(n/2)) делаем подсчёт младших значаших нулей
    for (int i = 0; i < 2 * (max_size - max_size / 2); i++)
        leasts_significant += "0";

    // Добавляем нули в конец строки, тем самым сдвигая разряды числа влево и обнуляем
    // строку значащих нулей для следующего сдвига разрядов
    P1.append(leasts_significant);
    bigint1.setResult(P1);
    leasts_significant = "";
    
    // Для сдвига разрядов влево ((P3 - P1 - P2) * 2^(n/2)) делаем подсчёт младших
    // значаших нулей
    for (int i = 0; i < max_size - max_size / 2; i++)
        leasts_significant += "0";

    // Добавляем нули в конец строки, тем самым сдвигая разряды числа влево и вычисляем
    // результат прозведения через сложения и вычитания
    P1_3.append(leasts_significant);
    bigint4.setResult(P1_3);
    result = (bigint4 + bigint1 + bigint2).getResult();
    result.erase(0, result.find_first_not_of('0'));
    this->setResult(result);

    // Возвращаем результирующую строку со стёртыми ведущими нулями
    return *this;
}

// Операция деления для двух длинных чисел (деление в столбик за
// время O(max(n,m)^2)
BigInteger & BigInteger::divide(BigInteger &other)
{
    // Размеры необходимых переменных
    std::string &first = this->getResult();
    std::string &second = other.getResult();
    int first_size = first.size();
    int second_size = second.size();
    int diff_size = first_size - second_size;

    // Проверяем базовые случаи
    if (second_size > first_size)
    {
	this->setResult("0");
	return *this;
    }

    if (second == "0" || second == "1")
    {
	return *this;
    }

    // Вычисляем наибольшую и наименьшую строки
    int min_size = std::min(first_size, second_size);
    int max_size = std::max(first_size, second_size);

    // Инициализируем начальные значения для деления
    std::string cnt = "0";
    BigInteger counter(cnt), number(cnt);
    number.setResult("1");

    // Если длина делимого и делителя отличается не более чем на 3 байта, можем
    // произвести наивное вычитание
    if (first_size <= 3 + second_size)
    {
	while (true)
	{
	    if (this->getResult()[0] == '-')
	    {
		*this = (*this - other);
		break;
	    }
	    
	    // Вычисляем остаток от деления
	    *this = (*this - other);
	    counter = counter + number;
	}

	// Устанавливаем результат деления
	this->setResult((counter - number).getResult());
    }
    else
    {
	// Если длина делимого и делителя отличается более чем на
	// 3 байта, реализуем алгоритм деления в столбик
	BigInteger curr_number(*this);
	std::string steel = this->getResult();
	std::string newString = "";
	int counter = 0;

	// Проходим вдоль строки делимого до конца
	while (steel[counter] != '\0')
	{
	    static int skip_counter = 0;
	    static std::string stool = "";
	    stool += first[counter++];
	    curr_number.setResult(stool);

	    // Если вычитание делителя из делимого отрицательно - значит
	    // делитель не вмещается под вынесенное значение - пропускаем
	    // и дописываем нули при необходимости, иначе вычисляем
	    // разность нескольких взятых делителей с вынесенным значением
	    if ((curr_number - other).getResult()[0] != '-')
	    {
		std::string num1 = "1";
		std::string num2 = "0";
		std::string last_positive = "";
		BigInteger integer1(num1);
		BigInteger integer2(num2);

		// Запоминаем последнее положительное число, т.к. именно оно является последним
		// не вмещаемым в делитель числом, в то же время производим вычисление частного
		while ((curr_number).getResult()[0] != '-')
		{
		    if (curr_number.getResult()[0] != '-')
			last_positive = curr_number.getResult();
		    
		    curr_number = curr_number - other;
		    integer2 = integer2 + integer1;
		}

		// Запоминаем перед перемещением указателя на следующий символ строки делимого
		// значение положительного не вмещенного в вынесенное выражение числа
		if (last_positive != "")
		    stool = last_positive;
		else
		    stool = "";

		// Вносим результат вычислений частного в строку
		newString += integer2.getResult();
		skip_counter = 0;
	    }
	    else
		skip_counter++;

	    // Дописываем нули в частное при необходимости (при занятии дополнительных позиций в числе)
	    if (skip_counter > 0)
	    {
		for (int i = 0; i < skip_counter; i++)
		    newString += "0";
	    }
	}

	// Записываем конечное частное в строку
	this->setResult(newString.erase(0, newString.find_first_not_of('0')));
    }
    
    return *this;
}

// Пример использования
int main()
{
    std::string str1 = "12837528735151289238535325235251351257197253";
    std::string str2 = "2219859812592109012982190129012";
    BigInteger integer1(str1);
    BigInteger integer2(str2);
    BigInteger result(integer1 / integer2);
    std::cout << result.getResult();   
}
