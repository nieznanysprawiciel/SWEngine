#pragma once


struct DeserializerImpl;

/**@brief Interfejs deserializatorów.*/
class IDeserializer
{
private:
	DeserializerImpl*		impl;
protected:
public:
	IDeserializer();
	~IDeserializer();

};


