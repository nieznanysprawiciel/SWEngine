#pragma once


struct DeserializerImpl;

/**@brief Interfejs deserializator�w.*/
class IDeserializer
{
private:
	DeserializerImpl*		impl;
protected:
public:
	IDeserializer();
	~IDeserializer();

};


