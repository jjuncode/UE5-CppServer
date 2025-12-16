#include "Network/NetUtils.h"

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

void SendBuffer::CopyData(void* data, int32 len)
{
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}