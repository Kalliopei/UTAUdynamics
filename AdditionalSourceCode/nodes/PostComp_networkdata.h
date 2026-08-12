namespace project
{

struct PostComp_networkdata: public scriptnode::dll::InterpretedNetworkData
{
	String getId() const override
	{
		return "PostComp";
	}
	bool isModNode() const override
	{
		return false;
	}
	String getNetworkData() const override
	{
		return "121.nT6K8CxkEN..xcDFZ.XpMvXJIcpMwymj5xokaDPfosnBP.MsQS9C+m2rSEbQ0+4iKUvzQF.2QRlwkFEdigIqylMF1McYHbOoQUiU6EhHv8DQS2czAcPFA2aJi2TwWTTCMqJyf6GRbbewpL5LEL..Yg.LgdDjV5BQA.";
	}
};
}

