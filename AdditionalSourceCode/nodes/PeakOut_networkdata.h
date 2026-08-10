namespace project
{

struct PeakOut_networkdata: public scriptnode::dll::InterpretedNetworkData
{
	String getId() const override
	{
		return "PeakOut";
	}
	bool isModNode() const override
	{
		return false;
	}
	String getNetworkData() const override
	{
		return "180.nT6K8C1I.TUA.HdBfzAXoNwNZKMQqPAFDTBqKN.B5DTDYKJTbhNgrAI8CLYos2kSwMuDUurYGmoipMJJcHp9uyEghihRvCFJi6BOtDbogxRqxaK.5aXobiUG+41GFVyO2NePlkA8OycopgSlEQ.8bCTQ02XMVaMP+C5jMq3yoDPKurHnGIwPOTCWLYk.O.BXBMnOPMnwxcx.+poe4BW4AbVSfacGLfGHXAs3qYgEvEhTjOQA";
	}
};
}

