namespace project
{

struct Peak_networkdata: public scriptnode::dll::InterpretedNetworkData
{
	String getId() const override
	{
		return "Peak";
	}
	bool isModNode() const override
	{
		return false;
	}
	String getNetworkData() const override
	{
		return "117.nT6K8CxhkM..xb3EZ.XpMvXJhk1563kE84zzYHPFsxb35.qEaoe.+m2rSEbQ0+YiKUvzQF.2ORlQEKEdigHqylMF1McYHbGoQ0xp8BOD3ddno6N5fNHyf6Mkwal3IXpLBta.gw8jUWzYJL..Qg.lOkfygANJ";
	}
};
}

