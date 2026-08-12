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
		return "122.nT6K8CBkMN..hcHFa.1ZaLXD4gzXgZjoNnSzGtQ.7JpfbSRwT0J+A7edyNYvEW+mOtjAUOc.bGIYFWzT3MELgg1rofcSYGB2SZb0rZyvCAtmHp5tmPIT5H3dSU3MU7lQhELWmA2Mf339xpM5TEv..bEBvBJWHRQNDE.";
	}
};
}

