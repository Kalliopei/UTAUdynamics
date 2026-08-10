#pragma once

// These will improve the readability of the connection definition

#define getT(Idx) template get<Idx>()
#define connectT(Idx, target) template connect<Idx>(target)
#define getParameterT(Idx) template getParameter<Idx>()
#define setParameterT(Idx, value) template setParameter<Idx>(value)
#define setParameterWT(Idx, value) template setWrapParameter<Idx>(value)
using namespace scriptnode;
using namespace snex;
using namespace snex::Types;

namespace DspNetwork_impl
{
// ==============================| Node & Parameter type declarations |==============================

using comp_t = wrap::no_data<dynamics::comp>;
DECLARE_PARAMETER_RANGE(pma1_modRange, 
                        50., 
                        4000.);

using pma1_mod = parameter::from0To1<comp_t, 
                                     2, 
                                     pma1_modRange>;

template <int NV>
using pma1_t = control::pma<NV, pma1_mod>;
template <int NV>
using envelope_follower_t = wrap::mod<parameter::plain<pma1_t<NV>, 0>, 
                                      wrap::no_data<dynamics::envelope_follower<NV>>>;

DECLARE_PARAMETER_RANGE_INV(envelope_follower1_modRange, 
                            -1., 
                            1.);

template <int NV>
using envelope_follower1_mod = parameter::from0To1_inv<pma1_t<NV>, 
                                                       2, 
                                                       envelope_follower1_modRange>;

template <int NV>
using envelope_follower1_t = wrap::mod<envelope_follower1_mod<NV>, 
                                       wrap::no_data<dynamics::envelope_follower<NV>>>;

DECLARE_PARAMETER_RANGE_STEP(pma_mod_0Range, 
                             -54., 
                             0., 
                             0.1);

using pma_mod_0 = parameter::from0To1<comp_t, 
                                      0, 
                                      pma_mod_0Range>;

DECLARE_PARAMETER_RANGE_SKEW_INV(pma_mod_1Range, 
                                 0., 
                                 28., 
                                 0.5);

template <int NV>
using pma_mod_1 = parameter::from0To1_inv<core::gain<NV>, 
                                          0, 
                                          pma_mod_1Range>;

template <int NV>
using pma_mod = parameter::chain<ranges::Identity, 
                                 pma_mod_0, 
                                 pma_mod_1<NV>>;

template <int NV>
using pma_t = control::pma<NV, pma_mod<NV>>;

namespace custom
{

struct expr2
{
	static float op(float input, float value)
	{
		return input >= 0.0 ? Math.tanh(2.0 * input) : Math.tanh((2.0 + value * 6.0) * input);;
	}
};
}

template <int NV>
using soft_bypass_t_ = container::chain<parameter::empty, 
                                        wrap::fix<2, math::expr<NV, custom::expr2>>, 
                                        core::gain<NV>>;

template <int NV>
using soft_bypass_t = bypass::smoothed<20, soft_bypass_t_<NV>>;
template <int NV>
using minmax_mod = parameter::bypass<soft_bypass_t<NV>>;

template <int NV>
using minmax_t = control::minmax<NV, minmax_mod<NV>>;
using comp1_t = comp_t;

template <int NV>
using oversample8x_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, envelope_follower_t<NV>>, 
                                         envelope_follower1_t<NV>, 
                                         pma1_t<NV>, 
                                         comp_t, 
                                         pma_t<NV>, 
                                         core::gain<NV>, 
                                         core::gain<NV>, 
                                         minmax_t<NV>, 
                                         soft_bypass_t<NV>, 
                                         comp1_t, 
                                         core::gain<NV>>;

template <int NV>
using oversample8x_t = wrap::oversample<8, oversample8x_t_<NV>>;

namespace DspNetwork_t_parameters
{
// Parameter list for DspNetwork_impl::DspNetwork_t ------------------------------------------------

DECLARE_PARAMETER_RANGE_INV(AmountRange, 
                            0., 
                            1.);

template <int NV>
using Amount = parameter::from0To1_inv<DspNetwork_impl::pma_t<NV>, 
                                       0, 
                                       AmountRange>;

DECLARE_PARAMETER_RANGE_STEP(Ratio_InputRange, 
                             1, 
                             2., 
                             0.01);
DECLARE_PARAMETER_RANGE(Ratio_0Range, 
                        1., 
                        2.);

using Ratio_0 = parameter::from0To1<DspNetwork_impl::comp1_t, 
                                    3, 
                                    Ratio_0Range>;

using Ratio = parameter::chain<Ratio_InputRange, Ratio_0>;

DECLARE_PARAMETER_RANGE_SKEW(Tight_1Range, 
                             0., 
                             1., 
                             100.);

template <int NV>
using Tight_1 = parameter::from0To1<DspNetwork_impl::minmax_t<NV>, 
                                    0, 
                                    Tight_1Range>;

DECLARE_PARAMETER_RANGE_STEP(Tight_2Range, 
                             0., 
                             24., 
                             0.1);

template <int NV>
using Tight_2 = parameter::from0To1<core::gain<NV>, 
                                    0, 
                                    Tight_2Range>;

DECLARE_PARAMETER_RANGE_SKEW_INV(Tight_3Range, 
                                 -18., 
                                 -6., 
                                 0.2);

template <int NV>
using Tight_3 = parameter::from0To1_inv<core::gain<NV>, 
                                        0, 
                                        Tight_3Range>;

template <int NV>
using Tight = parameter::chain<ranges::Identity, 
                               parameter::plain<math::expr<NV, custom::expr2>, 0>, 
                               Tight_1<NV>, 
                               Tight_2<NV>, 
                               Tight_3<NV>>;

DECLARE_PARAMETER_RANGE(Boost_InputRange, 
                        0., 
                        24.);
template <int NV> using Boost_0 = Tight_2<NV>;

template <int NV>
using Boost = parameter::chain<Boost_InputRange, Boost_0<NV>>;

template <int NV>
using DspNetwork_t_plist = parameter::list<Amount<NV>, 
                                           Ratio, 
                                           Tight<NV>, 
                                           Boost<NV>>;
}

template <int NV>
using DspNetwork_t_ = container::chain<DspNetwork_t_parameters::DspNetwork_t_plist<NV>, 
                                       wrap::fix<2, oversample8x_t<NV>>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public DspNetwork_impl::DspNetwork_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(DspNetwork);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(68)
		{
			0x005C, 0x0000, 0x0000, 0x6D41, 0x756F, 0x746E, 0x0000, 0x0000, 
            0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 
            0x0000, 0x005C, 0x0001, 0x0000, 0x6152, 0x6974, 0x006F, 0x0000, 
            0x8000, 0x003F, 0x0000, 0xCD40, 0x8CCC, 0x003F, 0x8000, 0x0A3F, 
            0x23D7, 0x5C3C, 0x0200, 0x0000, 0x5400, 0x6769, 0x7468, 0x0000, 
            0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 0x0000, 0x0000, 0x3F80, 
            0x0000, 0x0000, 0x005C, 0x0003, 0x0000, 0x6F42, 0x736F, 0x0074, 
            0x0000, 0x0000, 0x0000, 0xC000, 0x0041, 0xC000, 0x0040, 0x8000, 
            0x003F, 0x0000, 0x0000, 0x0000
		};
		SNEX_METADATA_ENCODED_MOD_INFO(2)
		{
			0x3D3B, 0x003E
		};
	};
	
	instance()
	{
		// Node References -------------------------------------------------------------------------
		
		auto& oversample8x = this->getT(0);               // DspNetwork_impl::oversample8x_t<NV>
		auto& envelope_follower = this->getT(0).getT(0);  // DspNetwork_impl::envelope_follower_t<NV>
		auto& envelope_follower1 = this->getT(0).getT(1); // DspNetwork_impl::envelope_follower1_t<NV>
		auto& pma1 = this->getT(0).getT(2);               // DspNetwork_impl::pma1_t<NV>
		auto& comp = this->getT(0).getT(3);               // DspNetwork_impl::comp_t
		auto& pma = this->getT(0).getT(4);                // DspNetwork_impl::pma_t<NV>
		auto& gain = this->getT(0).getT(5);               // core::gain<NV>
		auto& gain2 = this->getT(0).getT(6);              // core::gain<NV>
		auto& minmax = this->getT(0).getT(7);             // DspNetwork_impl::minmax_t<NV>
		auto& soft_bypass = this->getT(0).getT(8);        // DspNetwork_impl::soft_bypass_t<NV>
		auto& expr2 = this->getT(0).getT(8).getT(0);      // math::expr<NV, custom::expr2>
		auto& gain3 = this->getT(0).getT(8).getT(1);      // core::gain<NV>
		auto& comp1 = this->getT(0).getT(9);              // DspNetwork_impl::comp1_t
		auto& gain1 = this->getT(0).getT(10);             // core::gain<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		this->getParameterT(0).connectT(0, pma); // Amount -> pma::Value
		
		this->getParameterT(1).connectT(0, comp1); // Ratio -> comp1::Ratio
		
		auto& Tight_p = this->getParameterT(2);
		Tight_p.connectT(0, expr2);  // Tight -> expr2::Value
		Tight_p.connectT(1, minmax); // Tight -> minmax::Value
		Tight_p.connectT(2, gain2);  // Tight -> gain2::Gain
		Tight_p.connectT(3, gain3);  // Tight -> gain3::Gain
		
		this->getParameterT(3).connectT(0, gain1); // Boost -> gain1::Gain
		
		// Modulation Connections ------------------------------------------------------------------
		
		pma1.getWrappedObject().getParameter().connectT(0, comp);          // pma1 -> comp::Release
		envelope_follower.getParameter().connectT(0, pma1);                // envelope_follower -> pma1::Value
		envelope_follower1.getParameter().connectT(0, pma1);               // envelope_follower1 -> pma1::Add
		pma.getWrappedObject().getParameter().connectT(0, comp);           // pma -> comp::Threshhold
		pma.getWrappedObject().getParameter().connectT(1, gain);           // pma -> gain::Gain
		minmax.getWrappedObject().getParameter().connectT(0, soft_bypass); // minmax -> soft_bypass::Bypassed
		
		// Default Values --------------------------------------------------------------------------
		
		oversample8x.setParameterT(0, 1.); // container::chain::FilterType
		
		envelope_follower.setParameterT(0, 0.);   // dynamics::envelope_follower::Attack
		envelope_follower.setParameterT(1, 59.5); // dynamics::envelope_follower::Release
		envelope_follower.setParameterT(2, 0.);   // dynamics::envelope_follower::ProcessSignal
		
		envelope_follower1.setParameterT(0, 0.);    // dynamics::envelope_follower::Attack
		envelope_follower1.setParameterT(1, 4000.); // dynamics::envelope_follower::Release
		envelope_follower1.setParameterT(2, 0.);    // dynamics::envelope_follower::ProcessSignal
		
		;                           // pma1::Value is automated
		pma1.setParameterT(1, 0.5); // control::pma::Multiply
		;                           // pma1::Add is automated
		
		;                           // comp::Threshhold is automated
		comp.setParameterT(1, 1.);  // dynamics::comp::Attack
		;                           // comp::Release is automated
		comp.setParameterT(3, 2.1); // dynamics::comp::Ratio
		comp.setParameterT(4, 0.);  // dynamics::comp::Sidechain
		
		;                         // pma::Value is automated
		pma.setParameterT(1, 1.); // control::pma::Multiply
		pma.setParameterT(2, 0.); // control::pma::Add
		
		;                           // gain::Gain is automated
		gain.setParameterT(1, 20.); // core::gain::Smoothing
		gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                            // gain2::Gain is automated
		gain2.setParameterT(1, 20.); // core::gain::Smoothing
		gain2.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                            // minmax::Value is automated
		minmax.setParameterT(1, 0.); // control::minmax::Minimum
		minmax.setParameterT(2, 1.); // control::minmax::Maximum
		minmax.setParameterT(3, 1.); // control::minmax::Skew
		minmax.setParameterT(4, 0.); // control::minmax::Step
		minmax.setParameterT(5, 0.); // control::minmax::Polarity
		
		; // expr2::Value is automated
		
		;                            // gain3::Gain is automated
		gain3.setParameterT(1, 20.); // core::gain::Smoothing
		gain3.setParameterT(2, 0.);  // core::gain::ResetValue
		
		comp1.setParameterT(0, -100.); // dynamics::comp::Threshhold
		comp1.setParameterT(1, 0.);    // dynamics::comp::Attack
		comp1.setParameterT(2, 250.);  // dynamics::comp::Release
		;                              // comp1::Ratio is automated
		comp1.setParameterT(4, 0.);    // dynamics::comp::Sidechain
		
		;                            // gain1::Gain is automated
		gain1.setParameterT(1, 20.); // core::gain::Smoothing
		gain1.setParameterT(2, 0.);  // core::gain::ResetValue
		
		this->setParameterT(0, 1.);
		this->setParameterT(1, 1.1);
		this->setParameterT(2, 0.);
		this->setParameterT(3, 6.);
		this->setExternalData({}, -1);
	}
	~instance() override
	{
		// Cleanup external data references --------------------------------------------------------
		
		this->setExternalData({}, -1);
	}
	
	static constexpr bool isPolyphonic() { return NV > 1; };
	
	static constexpr bool hasTail() { return true; };
	
	static constexpr bool isSuspendedOnSilence() { return false; };
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(0).getT(0).setExternalData(b, index); // DspNetwork_impl::envelope_follower_t<NV>
		this->getT(0).getT(1).setExternalData(b, index); // DspNetwork_impl::envelope_follower1_t<NV>
		this->getT(0).getT(3).setExternalData(b, index); // DspNetwork_impl::comp_t
		this->getT(0).getT(9).setExternalData(b, index); // DspNetwork_impl::comp1_t
	}
};
}

#undef getT
#undef connectT
#undef setParameterT
#undef setParameterWT
#undef getParameterT
// ======================================| Public Definition |======================================

namespace project
{
// polyphonic template declaration

template <int NV>
using DspNetwork = wrap::node<DspNetwork_impl::instance<NV>>;
}


