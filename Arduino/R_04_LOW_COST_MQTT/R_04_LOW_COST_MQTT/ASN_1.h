#ifndef __ASN_1_H
#define __ASN_1_H
#include <Arduino.h>

//---Block identifier---//
#define BLOCK_CLASS_BITS          (uint8_t)0xC0
#define IS_EXIST_INNER_ASN_BLOCKS (uint8_t)0x20/*0 - only info about one meaning, 1 there is/are ASN blocks*/
#define BLOCK_TYPE_BITS           (uint8_t)0x1E/*Type Identifier of block*/
/*В случае если идентификатор типа для блока находится в диапазоне значений 0-30 идентификационный блок состоит только из одного октета. */
/*Часть общей длины блока содержит минимум 1 октет, кодирующий длину значения, которое содержит блок (именно только длину блока, содержащего закодированное значение, а не общую длину всего закодированного блока вместе с идентификатором блока и частью общей длины!). Длина блока в простейшем случае кодируется как без знаковое целое, разложенное по основанию 128. Бит 8 (старший бит) в этом случае является дополнительным флагом. Если общая длина закодированного блока превышает 128, то старший бит первого октета части общей длины блока должен быть установлен в 1, а следующие 7 бит должны кодировать без знаковое целое значение количества последующих октетов, которые и будут кодировать реальную общую длину блока.*/

#define ID_SEQUENCE               (uint8_t)0x30/*Используется для описания структуры данных, состоящей из различных типов*/
#define ID_INTEGER                (uint8_t)0x02/*Целое число*/
#define ID_OBJECT_IDENTIFIER      (uint8_t)0x06/*Последовательность целых чисел*/
#define ID_UTCTime                (uint8_t)0x17/*Временной тип, содержит 2 цифры для определения года / дата начала действия сертификата*/
#define ID_GeneralizedTime        (uint8_t)0x18/*Расширенный временной тип, содержит 4 цифры для обозначения года / дата окончания действия сертификата*/
#define ID_SET                    (uint8_t)0x31/*Описывает структуру данных разных типов*/
#define ID_UTF8String             (uint8_t)0x0C/*Описывает строковые данные*/
#define ID_NULL                   (uint8_t)0x05/*NULL*/
#define ID_BIT_STRING             (uint8_t)0x02/*Тип для хранения последовательности бит*/
#define ID_Context_Specific       (uint8_t)0xA0/*Тип для хранения последовательности бит*/

unsigned int getASNLenght(uint8_t* data,uint8_t** END)
{
	if(!(data[0]&0x80))return data[0];
  int i =0;
	
	if(data[0]==0x80)
	{
		while(data[i+1]||data[i+2])
		{
			i++;
			if(i==0xFFFFFFFF)
			{
				*END = &data[1];
				return 0;
			}
		}
		*END = &data[1];
		return i;
	}
	
	unsigned int length=0;
	
	for(i =0;i<data[0];i++)
	{
		length=length<<8|data[i+1];
	}
	*END = &data[i+1];
	return length;
}

uint8_t getUTCTime(uint8_t* startPosition, int dataLength, char* textFormatArray)//textFormatArray 13-09-15 15:35:02 UTC//22 byte
{
	uint8_t* endPtr=startPosition;
	while((uint32_t)endPtr<(uint32_t)(&startPosition[dataLength]))
	{
		if(startPosition[0]==ID_SEQUENCE)
		{
			int lenght = getASNLenght(&startPosition[1],&endPtr);
			uint8_t result=getUTCTime(endPtr,lenght,textFormatArray);
			if(result)return 1;//нашли
			else if((uint32_t)endPtr>=(uint32_t)(&startPosition[dataLength]))return 0;//данные закончились, результат - не нашли
			endPtr = &endPtr[lenght]; //не нашли, ищем дальше		
		}
		if(startPosition[0]==ID_UTCTime)
		{
			int lenght = getASNLenght(&startPosition[1],&endPtr);
			textFormatArray[0]=endPtr[0];
			textFormatArray[1]=endPtr[1];
			textFormatArray[2]='-';
			textFormatArray[3]=endPtr[2];
			textFormatArray[4]=endPtr[3];
			textFormatArray[5]='-';
			textFormatArray[6]=endPtr[4];
			textFormatArray[7]=endPtr[5];
			textFormatArray[8]=' ';
			textFormatArray[9]=endPtr[6];
			textFormatArray[10]=endPtr[7];
			textFormatArray[11]=':';
			textFormatArray[12]=endPtr[8];
			textFormatArray[13]=endPtr[9];
			textFormatArray[14]=':';
			textFormatArray[15]=endPtr[10];
			textFormatArray[16]=endPtr[11];
			textFormatArray[17]=' ';
			textFormatArray[18]='U';
			textFormatArray[19]='T';
			textFormatArray[20]='C';
			textFormatArray[21]=0x00;
			return 1;//нашли
		}
		else//skip all over
		{
			int lenght = getASNLenght(&startPosition[1],&endPtr);
			endPtr = &endPtr[lenght];
		}
	}
}

#endif
