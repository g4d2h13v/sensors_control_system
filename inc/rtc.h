#ifndef RTC_H
#define RTC_H

/*
** В этом модуле описаны подпрограммы для инициализации RTC и чтения счетчика
*/

#include <stdint.h>

/*
** Описание: Начальная конфигурация блока RTC
** Параметры: нет
** Возвращаемое значение: нет
*/
void rtc_init(void);

/*
** Описание: Получение текущего времени в секундах
** Параметры: нет
** Возвращаемое значение: Секунды, отсчитанные за текущий день
*/
uint32_t get_sec(void);

#endif
