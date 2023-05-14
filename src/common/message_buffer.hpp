#pragma once


#include <vector>
#include <stdint.h>
#include <string.h> //linux:memmove


namespace zq {


class MessageBuffer
{
	typedef std::vector<uint8_t>::size_type size_type;

public:
	MessageBuffer() :
			m_wpos(0), m_rpos(0), m_storage()
	{
		m_storage.resize(4096);
	}

	explicit MessageBuffer(std::size_t initialSize) :
			m_wpos(0), m_rpos(0), m_storage()
	{
		m_storage.resize(initialSize);
	}

	MessageBuffer(MessageBuffer const& right) :
			m_wpos(right.m_wpos), m_rpos(right.m_rpos), m_storage(right.m_storage)
	{
	}

	MessageBuffer(MessageBuffer&& right) :
			m_wpos(right.m_wpos), m_rpos(right.m_rpos), m_storage(right.move()) 
	{
	}

	void clear()
	{
		reset();
		m_storage.clear();
	}

	void reset()
	{
		m_wpos = 0;
		m_rpos = 0;
	}

	void resize(size_type bytes)
	{
		m_storage.resize(bytes);
	}

	bool empty() { return m_wpos == 0; }

	uint8_t* getBasePointer() { return m_storage.data(); }

	uint8_t* getReadPointer() { return getBasePointer() + m_rpos; }

	uint8_t* getWritePointer() { return getBasePointer() + m_wpos; }

	void readCompleted(size_type bytes) { m_rpos += bytes; }

	void writeCompleted(size_type bytes) { m_wpos += bytes; }

	size_type getActiveSize() const { return m_wpos - m_rpos; }

	size_type getRemainingSpace() const { return m_storage.size() - m_wpos; }

	size_type getBufferSize() const { return m_storage.size(); }

	void normalize()
	{
		if (m_rpos)
		{
			if (m_rpos != m_wpos)
			{
				memmove(getBasePointer(), getReadPointer(), getActiveSize());
			}

			m_wpos -= m_rpos;
			m_rpos = 0;
		}
	}

	void ensureFreeSpace()
	{
		if (getRemainingSpace() == 0)
		{
			m_storage.resize(m_storage.size() * 3 / 2);
		}
	}

	void write(void const* data, std::size_t size)
	{
		if (size)
		{
			memcpy(getWritePointer(), data, size);
			writeCompleted(size);
		}
	}

	std::vector<uint8_t>&& move()
	{
		m_wpos = 0;
		m_rpos = 0;
		return std::move(m_storage);
	}

	MessageBuffer& operator=(MessageBuffer const& right)
	{
		if (this != &right)
		{
			m_wpos = right.m_wpos;
			m_rpos = right.m_rpos;
			m_storage = right.m_storage;
		}

		return *this;
	}

	MessageBuffer& operator=(MessageBuffer&& right)
	{
		if (this != &right)
		{
			m_wpos = right.m_wpos;
			m_rpos = right.m_rpos;
			m_storage = right.move();
		}

		return *this;
	}

private:
	size_type m_wpos;
	size_type m_rpos;
	std::vector<uint8_t> m_storage;
};

}


