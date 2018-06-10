#ifndef HASHMAP_H
#define HASHMAP_H

#include <Arduino.h>

/* Handle association */
template <typename hash, typename map>
class HashType
{
  public:
	HashType() { reset(); }

	HashType(hash code, map value) : hashCode(code), mappedValue(value) {}

	void reset()
	{
		hashCode = 0;
		mappedValue = 0;
	}
	hash getHash() { return hashCode; }
	void setHash(hash code) { hashCode = code; }
	map getValue() { return mappedValue; }
	void setValue(map value) { mappedValue = value; }

	HashType &operator()(hash code, map value)
	{
		setHash(code);
		setValue(value);
	}

  private:
	hash hashCode;
	map mappedValue;
};

/*
Handle indexing and searches
TODO - extend API
*/
template <typename hash, typename map>
class HashMap
{
  public:
	HashMap(HashType<hash, map> *newMap, byte newSize)
	{
		hashMap = newMap;
		size = newSize;
		for (byte i = 0; i < size; i++)
		{
			hashMap[i].reset();
		}
	}

	HashType<hash, map> &operator[](int x)
	{
		//TODO - bounds
		return hashMap[x];
	}

	byte getIndexOf(hash key)
	{
		for (byte i = 0; i < size; i++)
		{
			if (hashMap[i].getHash() == key)
			{
				return i;
			}
		}

		return -1;
	}
	map getValueOf(hash key)
	{
		Serial.print(F("Get Value Of ->"));
		Serial.println(key);
		for (byte i = 0; i < size; i++)
		{
			Serial.println(hashMap[i].getHash());
			delay(100);
			if (hashMap[i].getHash() == key)
			{
				Serial.print(F("Getting value "));
				Serial.println(i);
				delay(100);
				map item = hashMap[i].getValue();
				Serial.print(F("Value retrieved"));
				delay(100);
				return item;
			}
		}

		return 0;
	}

  private:
	HashType<hash, map> *hashMap;
	byte size;
};

#endif