namespace project
{

struct PreComp_networkdata: public scriptnode::dll::InterpretedNetworkData
{
	String getId() const override
	{
		return "PreComp";
	}
	bool isModNode() const override
	{
		return false;
	}
	String getNetworkData() const override
	{
		return "120.nT6K8CBk8M..hc.FZ.WRb3bIGKKTRMw5u+9dJ.CDs.peUDYGMq7G+uY8osjS0+aCTaY5HyAdDs6TYgvcrLYcX0wxmaWlA3SdoZg22JO..uhrIDO5fNHS.7tk0rl3JssfxJi.7FPXfWAuKdZJC.vUH.KnbgHE4PT.";
	}
};
}

