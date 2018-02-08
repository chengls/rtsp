#ifndef HW_MEDIA_ENCODER_H
#define HW_MEDIA_ENCODER_H

struct StreamEncodedataParameter
{
	char *minetype;
	int width;
	int height;
	int fps;
	int gop;
	int bitrate;
	int channels;	
	int samplerate;
	int profile; //LC
};

class CHwMediaEncoder
{
public:
	CHwMediaEncoder() { m_codecid = -1;};
	virtual ~CHwMediaEncoder() {};

public: //��Ƶ���: aac����Ƶ���: h264
	virtual int Encode(unsigned char *pktBuf, int pktLen, int64_t &presentationTimeUs, unsigned char *outBuf) = 0;

public:
	int m_codecid; //��������ʽ
};

#endif

