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
		return "174.nT6K8C1G.TRA.HexevAXoNwtXOYQqPAFDTBqSIPPmfIDF+v.+ZircfR+SdZ6k42Pg0gdJ14on5kM+7n.oZkhRWhp+6bSnXo3D7PLx3dP7RCkm1k2V.z6vR4FqP9ysPMrletc9fMKB5glKSUCmMKhC8rCbQ02XMVaIP+C5jMm3QKyrBnOQwPeTCWL4kPCf.oPi8.XC5hUqhoG6RsME3J+BFt8QPyhilEyUBNGF9n.";
	}
};
}

