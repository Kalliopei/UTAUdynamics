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

namespace Dynamics_impl
{
// ==============================| Node & Parameter type declarations |==============================

using comp8_t = wrap::no_data<dynamics::comp>;
DECLARE_PARAMETER_RANGE_STEP(pma_modRange, 
                             250., 
                             3000., 
                             0.1);

using pma_mod = parameter::from0To1<comp8_t, 
                                    2, 
                                    pma_modRange>;

template <int NV>
using pma_t = control::pma<NV, pma_mod>;
template <int NV>
using envelope_follower_t = wrap::mod<parameter::plain<pma_t<NV>, 0>, 
                                      wrap::no_data<dynamics::envelope_follower<NV>>>;

DECLARE_PARAMETER_RANGE_INV(envelope_follower1_modRange, 
                            -1., 
                            1.);

template <int NV>
using envelope_follower1_mod = parameter::from0To1_inv<pma_t<NV>, 
                                                       2, 
                                                       envelope_follower1_modRange>;

template <int NV>
using envelope_follower1_t = wrap::mod<envelope_follower1_mod<NV>, 
                                       wrap::no_data<dynamics::envelope_follower<NV>>>;

template <int NV>
using chain6_t = container::chain<parameter::empty, 
                                  wrap::fix<2, envelope_follower_t<NV>>, 
                                  envelope_follower1_t<NV>, 
                                  pma_t<NV>>;

DECLARE_PARAMETER_RANGE_INV(smoothed_parameter_mod_0Range, 
                            0., 
                            1.);

template <int NV>
using smoothed_parameter_mod_0 = parameter::from0To1_inv<math::mul<NV>, 
                                                         0, 
                                                         smoothed_parameter_mod_0Range>;

template <int NV>
using smoothed_parameter_mod = parameter::chain<ranges::Identity, 
                                                smoothed_parameter_mod_0<NV>, 
                                                parameter::plain<math::mul<NV>, 0>>;

template <int NV>
using smoothed_parameter_t = wrap::mod<smoothed_parameter_mod<NV>, 
                                       control::smoothed_parameter<NV, smoothers::linear_ramp<NV>>>;
template <int NV>
using minmax_t = control::minmax<NV, 
                                 parameter::plain<smoothed_parameter_t<NV>, 0>>;

template <int NV>
using chain11_t = container::chain<parameter::empty, 
                                   wrap::fix<2, minmax_t<NV>>, 
                                   smoothed_parameter_t<NV>, 
                                   math::mul<NV>>;
using comp2_t = comp8_t;

template <int NV>
using chain2_t = container::chain<parameter::empty, 
                                  wrap::fix<2, filters::biquad<NV>>, 
                                  comp2_t, 
                                  core::gain<NV>>;
using comp1_t = comp8_t;

template <int NV>
using chain1_t = container::chain<parameter::empty, 
                                  wrap::fix<2, filters::biquad<NV>>, 
                                  filters::biquad<NV>, 
                                  comp1_t, 
                                  core::gain<NV>>;
using comp3_t = comp8_t;

template <int NV>
using chain3_t = container::chain<parameter::empty, 
                                  wrap::fix<2, filters::biquad<NV>>, 
                                  filters::biquad<NV>, 
                                  comp3_t, 
                                  core::gain<NV>>;
using comp4_t = comp8_t;

template <int NV>
using chain4_t = container::chain<parameter::empty, 
                                  wrap::fix<2, filters::biquad<NV>>, 
                                  filters::biquad<NV>, 
                                  comp4_t, 
                                  core::gain<NV>>;
using comp5_t = comp8_t;

template <int NV>
using chain5_t = container::chain<parameter::empty, 
                                  wrap::fix<2, filters::biquad<NV>>, 
                                  filters::biquad<NV>, 
                                  comp5_t, 
                                  core::gain<NV>>;
using comp7_t = comp8_t;

template <int NV>
using chain7_t = container::chain<parameter::empty, 
                                  wrap::fix<2, filters::biquad<NV>>, 
                                  comp7_t, 
                                  core::gain<NV>>;

template <int NV>
using split_t = container::split<parameter::empty, 
                                 wrap::fix<2, chain2_t<NV>>, 
                                 chain1_t<NV>, 
                                 chain3_t<NV>, 
                                 chain4_t<NV>, 
                                 chain5_t<NV>, 
                                 chain7_t<NV>>;

template <int NV>
using chain10_t = container::chain<parameter::empty, 
                                   wrap::fix<2, split_t<NV>>, 
                                   math::mul<NV>>;

namespace split1_t_parameters
{
}

template <int NV>
using split1_t = container::split<parameter::plain<Dynamics_impl::minmax_t<NV>, 0>, 
                                  wrap::fix<2, chain11_t<NV>>, 
                                  chain10_t<NV>>;

using chain_t = container::chain<parameter::empty, wrap::fix<2, comp8_t>>;

template <int NV>
using split3_t = container::split<parameter::empty, 
                                  wrap::fix<2, core::gain<NV>>, 
                                  math::abs<NV>>;

template <int NV>
using chain13_t = container::chain<parameter::empty, 
                                   wrap::fix<2, split3_t<NV>>, 
                                   core::gain<NV>, 
                                   math::tanh<NV>>;

template <int NV>
using chain16_t = container::chain<parameter::empty, 
                                   wrap::fix<2, math::abs<NV>>, 
                                   math::mul<NV>>;

template <int NV>
using split4_t = container::split<parameter::empty, 
                                  wrap::fix<2, core::gain<NV>>, 
                                  chain16_t<NV>>;

template <int NV>
using chain15_t = container::chain<parameter::empty, 
                                   wrap::fix<2, split4_t<NV>>, 
                                   core::gain<NV>, 
                                   core::gain<NV>, 
                                   math::tanh<NV>>;

template <int NV>
using chain14_t = container::chain<parameter::empty, 
                                   wrap::fix<2, chain15_t<NV>>>;

template <int NV>
using split2_t = container::split<parameter::empty, 
                                  wrap::fix<2, chain13_t<NV>>, 
                                  chain14_t<NV>>;

template <int NV>
using chain12_t = container::chain<parameter::empty, 
                                   wrap::fix<2, split2_t<NV>>>;

template <int NV>
using chain8_t = container::chain<parameter::empty, 
                                  wrap::fix<2, core::gain<NV>>, 
                                  chain12_t<NV>, 
                                  core::gain<NV>>;
using comp_t = comp8_t;

using chain9_t = container::chain<parameter::empty, wrap::fix<2, comp_t>>;
template <int NV>
using oversample_t_ = container::chain<parameter::empty, 
                                       wrap::fix<2, chain6_t<NV>>, 
                                       split1_t<NV>, 
                                       chain_t, 
                                       chain8_t<NV>, 
                                       chain9_t>;

template <int NV>
using oversample_t = wrap::oversample<0, oversample_t_<NV>>;

namespace Dynamics_t_parameters
{
// Parameter list for Dynamics_impl::Dynamics_t ----------------------------------------------------

DECLARE_PARAMETER_RANGE_SKEW(Amount_0Range, 
                             2.1, 
                             3.6, 
                             0.296803);

using Amount_0 = parameter::from0To1<Dynamics_impl::comp8_t, 
                                     3, 
                                     Amount_0Range>;

DECLARE_PARAMETER_RANGE_STEP_INV(Amount_1Range, 
                                 -54., 
                                 0., 
                                 0.1);

using Amount_1 = parameter::from0To1_inv<Dynamics_impl::comp8_t, 
                                         0, 
                                         Amount_1Range>;

DECLARE_PARAMETER_RANGE(Amount_2Range, 
                        -1., 
                        1.);

template <int NV>
using Amount_2 = parameter::from0To1<Dynamics_impl::pma_t<NV>, 
                                     1, 
                                     Amount_2Range>;

template <int NV>
using Amount = parameter::chain<ranges::Identity, 
                                Amount_0, 
                                Amount_1, 
                                Amount_2<NV>>;

DECLARE_PARAMETER_RANGE_STEP(Color_1Range, 
                             0., 
                             6., 
                             0.1);

template <int NV>
using Color_1 = parameter::from0To1<core::gain<NV>, 
                                    0, 
                                    Color_1Range>;

DECLARE_PARAMETER_RANGE_SKEW_INV(Color_2Range, 
                                 -12., 
                                 0., 
                                 2.);

template <int NV>
using Color_2 = parameter::from0To1_inv<core::gain<NV>, 
                                        0, 
                                        Color_2Range>;

DECLARE_PARAMETER_RANGE_SKEW(Color_3Range, 
                             0., 
                             6., 
                             0.2);

template <int NV>
using Color_3 = parameter::from0To1<core::gain<NV>, 
                                    0, 
                                    Color_3Range>;

template <int NV>
using Color = parameter::chain<ranges::Identity, 
                               parameter::plain<Dynamics_impl::split1_t<NV>, 0>, 
                               Color_1<NV>, 
                               Color_2<NV>, 
                               Color_3<NV>>;

using Clamp = parameter::plain<Dynamics_impl::comp_t, 3>;
using Boost = parameter::empty;
template <int NV>
using Dynamics_t_plist = parameter::list<Amount<NV>, 
                                         Color<NV>, 
                                         Clamp, 
                                         Boost>;
}

template <int NV>
using Dynamics_t_ = container::chain<Dynamics_t_parameters::Dynamics_t_plist<NV>, 
                                     wrap::fix<2, oversample_t<NV>>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public Dynamics_impl::Dynamics_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 0;
		
		SNEX_METADATA_ID(Dynamics);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(68)
		{
			0x005C, 0x0000, 0x0000, 0x6D41, 0x756F, 0x746E, 0x0000, 0x0000, 
            0x0000, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000, 
            0x0000, 0x005C, 0x0001, 0x0000, 0x6F43, 0x6F6C, 0x0072, 0x0000, 
            0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 0x0000, 0x8000, 0x003F, 
            0x0000, 0x5C00, 0x0200, 0x0000, 0x4300, 0x616C, 0x706D, 0x0000, 
            0x0000, 0x3F80, 0x0000, 0x4000, 0xCCCD, 0x3F8C, 0x0000, 0x3F80, 
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
		
		auto& oversample = this->getT(0);                                             // Dynamics_impl::oversample_t<NV>
		auto& chain6 = this->getT(0).getT(0);                                         // Dynamics_impl::chain6_t<NV>
		auto& envelope_follower = this->getT(0).getT(0).getT(0);                      // Dynamics_impl::envelope_follower_t<NV>
		auto& envelope_follower1 = this->getT(0).getT(0).getT(1);                     // Dynamics_impl::envelope_follower1_t<NV>
		auto& pma = this->getT(0).getT(0).getT(2);                                    // Dynamics_impl::pma_t<NV>
		auto& split1 = this->getT(0).getT(1);                                         // Dynamics_impl::split1_t<NV>
		auto& chain11 = this->getT(0).getT(1).getT(0);                                // Dynamics_impl::chain11_t<NV>
		auto& minmax = this->getT(0).getT(1).getT(0).getT(0);                         // Dynamics_impl::minmax_t<NV>
		auto& smoothed_parameter = this->getT(0).getT(1).getT(0).getT(1);             // Dynamics_impl::smoothed_parameter_t<NV>
		auto& dummy1 = this->getT(0).getT(1).getT(0).getT(2);                         // math::mul<NV>
		auto& chain10 = this->getT(0).getT(1).getT(1);                                // Dynamics_impl::chain10_t<NV>
		auto& split = this->getT(0).getT(1).getT(1).getT(0);                          // Dynamics_impl::split_t<NV>
		auto& chain2 = this->getT(0).getT(1).getT(1).getT(0).getT(0);                 // Dynamics_impl::chain2_t<NV>
		auto& biquad4 = this->getT(0).getT(1).getT(1).getT(0).getT(0).getT(0);        // filters::biquad<NV>
		auto& comp2 = this->getT(0).getT(1).getT(1).getT(0).getT(0).getT(1);          // Dynamics_impl::comp2_t
		auto& gain2 = this->getT(0).getT(1).getT(1).getT(0).getT(0).getT(2);          // core::gain<NV>
		auto& chain1 = this->getT(0).getT(1).getT(1).getT(0).getT(1);                 // Dynamics_impl::chain1_t<NV>
		auto& biquad2 = this->getT(0).getT(1).getT(1).getT(0).getT(1).getT(0);        // filters::biquad<NV>
		auto& biquad3 = this->getT(0).getT(1).getT(1).getT(0).getT(1).getT(1);        // filters::biquad<NV>
		auto& comp1 = this->getT(0).getT(1).getT(1).getT(0).getT(1).getT(2);          // Dynamics_impl::comp1_t
		auto& gain1 = this->getT(0).getT(1).getT(1).getT(0).getT(1).getT(3);          // core::gain<NV>
		auto& chain3 = this->getT(0).getT(1).getT(1).getT(0).getT(2);                 // Dynamics_impl::chain3_t<NV>
		auto& biquad6 = this->getT(0).getT(1).getT(1).getT(0).getT(2).getT(0);        // filters::biquad<NV>
		auto& biquad7 = this->getT(0).getT(1).getT(1).getT(0).getT(2).getT(1);        // filters::biquad<NV>
		auto& comp3 = this->getT(0).getT(1).getT(1).getT(0).getT(2).getT(2);          // Dynamics_impl::comp3_t
		auto& gain3 = this->getT(0).getT(1).getT(1).getT(0).getT(2).getT(3);          // core::gain<NV>
		auto& chain4 = this->getT(0).getT(1).getT(1).getT(0).getT(3);                 // Dynamics_impl::chain4_t<NV>
		auto& biquad8 = this->getT(0).getT(1).getT(1).getT(0).getT(3).getT(0);        // filters::biquad<NV>
		auto& biquad9 = this->getT(0).getT(1).getT(1).getT(0).getT(3).getT(1);        // filters::biquad<NV>
		auto& comp4 = this->getT(0).getT(1).getT(1).getT(0).getT(3).getT(2);          // Dynamics_impl::comp4_t
		auto& gain4 = this->getT(0).getT(1).getT(1).getT(0).getT(3).getT(3);          // core::gain<NV>
		auto& chain5 = this->getT(0).getT(1).getT(1).getT(0).getT(4);                 // Dynamics_impl::chain5_t<NV>
		auto& biquad10 = this->getT(0).getT(1).getT(1).getT(0).getT(4).getT(0);       // filters::biquad<NV>
		auto& biquad11 = this->getT(0).getT(1).getT(1).getT(0).getT(4).getT(1);       // filters::biquad<NV>
		auto& comp5 = this->getT(0).getT(1).getT(1).getT(0).getT(4).getT(2);          // Dynamics_impl::comp5_t
		auto& gain5 = this->getT(0).getT(1).getT(1).getT(0).getT(4).getT(3);          // core::gain<NV>
		auto& chain7 = this->getT(0).getT(1).getT(1).getT(0).getT(5);                 // Dynamics_impl::chain7_t<NV>
		auto& biquad14 = this->getT(0).getT(1).getT(1).getT(0).getT(5).getT(0);       // filters::biquad<NV>
		auto& comp7 = this->getT(0).getT(1).getT(1).getT(0).getT(5).getT(1);          // Dynamics_impl::comp7_t
		auto& gain7 = this->getT(0).getT(1).getT(1).getT(0).getT(5).getT(2);          // core::gain<NV>
		auto& dummy3 = this->getT(0).getT(1).getT(1).getT(1);                         // math::mul<NV>
		auto& chain = this->getT(0).getT(2);                                          // Dynamics_impl::chain_t
		auto& comp8 = this->getT(0).getT(2).getT(0);                                  // Dynamics_impl::comp8_t
		auto& chain8 = this->getT(0).getT(3);                                         // Dynamics_impl::chain8_t<NV>
		auto& gain8 = this->getT(0).getT(3).getT(0);                                  // core::gain<NV>
		auto& chain12 = this->getT(0).getT(3).getT(1);                                // Dynamics_impl::chain12_t<NV>
		auto& split2 = this->getT(0).getT(3).getT(1).getT(0);                         // Dynamics_impl::split2_t<NV>
		auto& chain13 = this->getT(0).getT(3).getT(1).getT(0).getT(0);                // Dynamics_impl::chain13_t<NV>
		auto& split3 = this->getT(0).getT(3).getT(1).getT(0).getT(0).getT(0);         // Dynamics_impl::split3_t<NV>
		auto& gain = this->getT(0).getT(3).getT(1).getT(0).getT(0).getT(0).getT(0);   // core::gain<NV>
		auto& abs = this->getT(0).getT(3).getT(1).getT(0).getT(0).getT(0).getT(1);    // math::abs<NV>
		auto& gain6 = this->getT(0).getT(3).getT(1).getT(0).getT(0).getT(1);          // core::gain<NV>
		auto& tanh = this->getT(0).getT(3).getT(1).getT(0).getT(0).getT(2);           // math::tanh<NV>
		auto& chain14 = this->getT(0).getT(3).getT(1).getT(0).getT(1);                // Dynamics_impl::chain14_t<NV>
		auto& chain15 = this->getT(0).getT(3).getT(1).getT(0).getT(1).getT(0);        // Dynamics_impl::chain15_t<NV>
		auto& split4 = this->getT(0).getT(3).getT(1).getT(0).getT(1).getT(0).getT(0); // Dynamics_impl::split4_t<NV>
		auto& gain10 = this->getT(0).getT(3).getT(1).getT(0).                         // core::gain<NV>
                       getT(1).getT(0).getT(0).getT(0);
		auto& chain16 = this->getT(0).getT(3).getT(1).getT(0).                        // Dynamics_impl::chain16_t<NV>
                        getT(1).getT(0).getT(0).getT(1);
		auto& abs1 = this->getT(0).getT(3).getT(1).getT(0).                           // math::abs<NV>
                     getT(1).getT(0).getT(0).getT(1).
                     getT(0);
		auto& mul = this->getT(0).getT(3).getT(1).getT(0).                            // math::mul<NV>
                    getT(1).getT(0).getT(0).getT(1).
                    getT(1);
		auto& gain11 = this->getT(0).getT(3).getT(1).getT(0).getT(1).getT(0).getT(1); // core::gain<NV>
		auto& gain12 = this->getT(0).getT(3).getT(1).getT(0).getT(1).getT(0).getT(2); // core::gain<NV>
		auto& tanh1 = this->getT(0).getT(3).getT(1).getT(0).getT(1).getT(0).getT(3);  // math::tanh<NV>
		auto& gain9 = this->getT(0).getT(3).getT(2);                                  // core::gain<NV>
		auto& chain9 = this->getT(0).getT(4);                                         // Dynamics_impl::chain9_t
		auto& comp = this->getT(0).getT(4).getT(0);                                   // Dynamics_impl::comp_t
		
		// Parameter Connections -------------------------------------------------------------------
		
		split1.getParameterT(0).connectT(0, minmax); // Blend -> minmax::Value
		auto& Amount_p = this->getParameterT(0);
		Amount_p.connectT(0, comp8); // Amount -> comp8::Ratio
		Amount_p.connectT(1, comp8); // Amount -> comp8::Threshhold
		Amount_p.connectT(2, pma);   // Amount -> pma::Multiply
		
		auto& Color_p = this->getParameterT(1);
		Color_p.connectT(0, split1); // Color -> split1::Blend
		Color_p.connectT(1, gain8);  // Color -> gain8::Gain
		Color_p.connectT(2, gain9);  // Color -> gain9::Gain
		Color_p.connectT(3, gain12); // Color -> gain12::Gain
		
		this->getParameterT(2).connectT(0, comp); // Clamp -> comp::Ratio
		
		// Modulation Connections ------------------------------------------------------------------
		
		pma.getWrappedObject().getParameter().connectT(0, comp8);                 // pma -> comp8::Release
		envelope_follower.getParameter().connectT(0, pma);                        // envelope_follower -> pma::Value
		envelope_follower1.getParameter().connectT(0, pma);                       // envelope_follower1 -> pma::Add
		smoothed_parameter.getParameter().connectT(0, dummy1);                    // smoothed_parameter -> dummy1::Value
		smoothed_parameter.getParameter().connectT(1, dummy3);                    // smoothed_parameter -> dummy3::Value
		minmax.getWrappedObject().getParameter().connectT(0, smoothed_parameter); // minmax -> smoothed_parameter::Value
		
		// Default Values --------------------------------------------------------------------------
		
		oversample.setParameterT(0, 2.); // container::chain::Oversampling
		oversample.setParameterT(1, 1.); // container::chain::FilterType
		
		envelope_follower.setParameterT(0, 0.);  // dynamics::envelope_follower::Attack
		envelope_follower.setParameterT(1, 60.); // dynamics::envelope_follower::Release
		envelope_follower.setParameterT(2, 0.);  // dynamics::envelope_follower::ProcessSignal
		
		envelope_follower1.setParameterT(0, 0.);    // dynamics::envelope_follower::Attack
		envelope_follower1.setParameterT(1, 3000.); // dynamics::envelope_follower::Release
		envelope_follower1.setParameterT(2, 0.);    // dynamics::envelope_follower::ProcessSignal
		
		; // pma::Value is automated
		; // pma::Multiply is automated
		; // pma::Add is automated
		
		; // split1::Blend is automated
		
		;                                 // minmax::Value is automated
		minmax.setParameterT(1, 0.);      // control::minmax::Minimum
		minmax.setParameterT(2, 1.);      // control::minmax::Maximum
		minmax.setParameterT(3, 1.98998); // control::minmax::Skew
		minmax.setParameterT(4, 0.);      // control::minmax::Step
		minmax.setParameterT(5, 0.);      // control::minmax::Polarity
		
		;                                          // smoothed_parameter::Value is automated
		smoothed_parameter.setParameterT(1, 100.); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter.setParameterT(2, 1.);   // control::smoothed_parameter::Enabled
		
		; // dummy1::Value is automated
		
		biquad4.setParameterT(0, 1000.); // filters::biquad::Frequency
		biquad4.setParameterT(1, 1.);    // filters::biquad::Q
		biquad4.setParameterT(2, 0.);    // filters::biquad::Gain
		biquad4.setParameterT(3, 0.01);  // filters::biquad::Smoothing
		biquad4.setParameterT(4, 5.);    // filters::biquad::Mode
		biquad4.setParameterT(5, 1.);    // filters::biquad::Enabled
		
		comp2.setParameterT(0, -60.); // dynamics::comp::Threshhold
		comp2.setParameterT(1, 1);    // dynamics::comp::Attack
		comp2.setParameterT(2, 250.); // dynamics::comp::Release
		comp2.setParameterT(3, 1.1);  // dynamics::comp::Ratio
		comp2.setParameterT(4, 0.);   // dynamics::comp::Sidechain
		
		gain2.setParameterT(0, 2.3); // core::gain::Gain
		gain2.setParameterT(1, 20.); // core::gain::Smoothing
		gain2.setParameterT(2, 0.);  // core::gain::ResetValue
		
		biquad2.setParameterT(0, 2494.31); // filters::biquad::Frequency
		biquad2.setParameterT(1, 1.);      // filters::biquad::Q
		biquad2.setParameterT(2, 0.);      // filters::biquad::Gain
		biquad2.setParameterT(3, 0.01);    // filters::biquad::Smoothing
		biquad2.setParameterT(4, 5.);      // filters::biquad::Mode
		biquad2.setParameterT(5, 1.);      // filters::biquad::Enabled
		
		biquad3.setParameterT(0, 348.249); // filters::biquad::Frequency
		biquad3.setParameterT(1, 1.);      // filters::biquad::Q
		biquad3.setParameterT(2, 0.);      // filters::biquad::Gain
		biquad3.setParameterT(3, 0.01);    // filters::biquad::Smoothing
		biquad3.setParameterT(4, 1.);      // filters::biquad::Mode
		biquad3.setParameterT(5, 1.);      // filters::biquad::Enabled
		
		comp1.setParameterT(0, -60.); // dynamics::comp::Threshhold
		comp1.setParameterT(1, 1);    // dynamics::comp::Attack
		comp1.setParameterT(2, 250.); // dynamics::comp::Release
		comp1.setParameterT(3, 1.3);  // dynamics::comp::Ratio
		comp1.setParameterT(4, 0.);   // dynamics::comp::Sidechain
		
		gain1.setParameterT(0, 2.8); // core::gain::Gain
		gain1.setParameterT(1, 20.); // core::gain::Smoothing
		gain1.setParameterT(2, 0.);  // core::gain::ResetValue
		
		biquad6.setParameterT(0, 4965.88); // filters::biquad::Frequency
		biquad6.setParameterT(1, 1.);      // filters::biquad::Q
		biquad6.setParameterT(2, 0.);      // filters::biquad::Gain
		biquad6.setParameterT(3, 0.01);    // filters::biquad::Smoothing
		biquad6.setParameterT(4, 5.);      // filters::biquad::Mode
		biquad6.setParameterT(5, 1.);      // filters::biquad::Enabled
		
		biquad7.setParameterT(0, 1000.); // filters::biquad::Frequency
		biquad7.setParameterT(1, 1.);    // filters::biquad::Q
		biquad7.setParameterT(2, 0.);    // filters::biquad::Gain
		biquad7.setParameterT(3, 0.01);  // filters::biquad::Smoothing
		biquad7.setParameterT(4, 1.);    // filters::biquad::Mode
		biquad7.setParameterT(5, 1.);    // filters::biquad::Enabled
		
		comp3.setParameterT(0, -60.); // dynamics::comp::Threshhold
		comp3.setParameterT(1, 1);    // dynamics::comp::Attack
		comp3.setParameterT(2, 250.); // dynamics::comp::Release
		comp3.setParameterT(3, 1.2);  // dynamics::comp::Ratio
		comp3.setParameterT(4, 0.);   // dynamics::comp::Sidechain
		
		gain3.setParameterT(0, 3.5); // core::gain::Gain
		gain3.setParameterT(1, 20.); // core::gain::Smoothing
		gain3.setParameterT(2, 0.);  // core::gain::ResetValue
		
		biquad8.setParameterT(0, 8692.6); // filters::biquad::Frequency
		biquad8.setParameterT(1, 1.);     // filters::biquad::Q
		biquad8.setParameterT(2, 0.);     // filters::biquad::Gain
		biquad8.setParameterT(3, 0.01);   // filters::biquad::Smoothing
		biquad8.setParameterT(4, 5.);     // filters::biquad::Mode
		biquad8.setParameterT(5, 1.);     // filters::biquad::Enabled
		
		biquad9.setParameterT(0, 2106.73); // filters::biquad::Frequency
		biquad9.setParameterT(1, 1.);      // filters::biquad::Q
		biquad9.setParameterT(2, 0.);      // filters::biquad::Gain
		biquad9.setParameterT(3, 0.01);    // filters::biquad::Smoothing
		biquad9.setParameterT(4, 1.);      // filters::biquad::Mode
		biquad9.setParameterT(5, 1.);      // filters::biquad::Enabled
		
		comp4.setParameterT(0, -60.); // dynamics::comp::Threshhold
		comp4.setParameterT(1, 1);    // dynamics::comp::Attack
		comp4.setParameterT(2, 250.); // dynamics::comp::Release
		comp4.setParameterT(3, 1.1);  // dynamics::comp::Ratio
		comp4.setParameterT(4, 0.);   // dynamics::comp::Sidechain
		
		gain4.setParameterT(0, 4.8); // core::gain::Gain
		gain4.setParameterT(1, 20.); // core::gain::Smoothing
		gain4.setParameterT(2, 0.);  // core::gain::ResetValue
		
		biquad10.setParameterT(0, 20000.); // filters::biquad::Frequency
		biquad10.setParameterT(1, 1.);     // filters::biquad::Q
		biquad10.setParameterT(2, 0.);     // filters::biquad::Gain
		biquad10.setParameterT(3, 0.01);   // filters::biquad::Smoothing
		biquad10.setParameterT(4, 5.);     // filters::biquad::Mode
		biquad10.setParameterT(5, 1.);     // filters::biquad::Enabled
		
		biquad11.setParameterT(0, 4881.91); // filters::biquad::Frequency
		biquad11.setParameterT(1, 1.);      // filters::biquad::Q
		biquad11.setParameterT(2, 0.);      // filters::biquad::Gain
		biquad11.setParameterT(3, 0.01);    // filters::biquad::Smoothing
		biquad11.setParameterT(4, 1.);      // filters::biquad::Mode
		biquad11.setParameterT(5, 1.);      // filters::biquad::Enabled
		
		comp5.setParameterT(0, -60.); // dynamics::comp::Threshhold
		comp5.setParameterT(1, 1);    // dynamics::comp::Attack
		comp5.setParameterT(2, 250.); // dynamics::comp::Release
		comp5.setParameterT(3, 2);    // dynamics::comp::Ratio
		comp5.setParameterT(4, 0.);   // dynamics::comp::Sidechain
		
		gain5.setParameterT(0, 2);   // core::gain::Gain
		gain5.setParameterT(1, 20.); // core::gain::Smoothing
		gain5.setParameterT(2, 0.);  // core::gain::ResetValue
		
		biquad14.setParameterT(0, 20000.); // filters::biquad::Frequency
		biquad14.setParameterT(1, 1.);     // filters::biquad::Q
		biquad14.setParameterT(2, 0.);     // filters::biquad::Gain
		biquad14.setParameterT(3, 0.01);   // filters::biquad::Smoothing
		biquad14.setParameterT(4, 1.);     // filters::biquad::Mode
		biquad14.setParameterT(5, 1.);     // filters::biquad::Enabled
		
		comp7.setParameterT(0, -60.); // dynamics::comp::Threshhold
		comp7.setParameterT(1, 1);    // dynamics::comp::Attack
		comp7.setParameterT(2, 250.); // dynamics::comp::Release
		comp7.setParameterT(3, 1.6);  // dynamics::comp::Ratio
		comp7.setParameterT(4, 0.);   // dynamics::comp::Sidechain
		
		gain7.setParameterT(0, 4.5); // core::gain::Gain
		gain7.setParameterT(1, 20.); // core::gain::Smoothing
		gain7.setParameterT(2, 0.);  // core::gain::ResetValue
		
		; // dummy3::Value is automated
		
		;                           // comp8::Threshhold is automated
		comp8.setParameterT(1, 1);  // dynamics::comp::Attack
		;                           // comp8::Release is automated
		;                           // comp8::Ratio is automated
		comp8.setParameterT(4, 0.); // dynamics::comp::Sidechain
		
		;                            // gain8::Gain is automated
		gain8.setParameterT(1, 20.); // core::gain::Smoothing
		gain8.setParameterT(2, 0.);  // core::gain::ResetValue
		
		gain.setParameterT(0, 0.);  // core::gain::Gain
		gain.setParameterT(1, 20.); // core::gain::Smoothing
		gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		abs.setParameterT(0, 1.); // math::abs::Value
		
		gain6.setParameterT(0, -6.); // core::gain::Gain
		gain6.setParameterT(1, 20.); // core::gain::Smoothing
		gain6.setParameterT(2, 0.);  // core::gain::ResetValue
		
		tanh.setParameterT(0, 1.); // math::tanh::Value
		
		gain10.setParameterT(0, 0.);  // core::gain::Gain
		gain10.setParameterT(1, 20.); // core::gain::Smoothing
		gain10.setParameterT(2, 0.);  // core::gain::ResetValue
		
		abs1.setParameterT(0, 1.); // math::abs::Value
		
		mul.setParameterT(0, -1.); // math::mul::Value
		
		gain11.setParameterT(0, -6);  // core::gain::Gain
		gain11.setParameterT(1, 20.); // core::gain::Smoothing
		gain11.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                             // gain12::Gain is automated
		gain12.setParameterT(1, 20.); // core::gain::Smoothing
		gain12.setParameterT(2, 0.);  // core::gain::ResetValue
		
		tanh1.setParameterT(0, 1.); // math::tanh::Value
		
		;                            // gain9::Gain is automated
		gain9.setParameterT(1, 20.); // core::gain::Smoothing
		gain9.setParameterT(2, 0.);  // core::gain::ResetValue
		
		comp.setParameterT(0, -100.); // dynamics::comp::Threshhold
		comp.setParameterT(1, 50.);   // dynamics::comp::Attack
		comp.setParameterT(2, 250.);  // dynamics::comp::Release
		;                             // comp::Ratio is automated
		comp.setParameterT(4, 0.);    // dynamics::comp::Sidechain
		
		this->setParameterT(0, 1.);
		this->setParameterT(1, 0.);
		this->setParameterT(2, 1.1);
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
		
		this->getT(0).getT(0).getT(0).setExternalData(b, index);                         // Dynamics_impl::envelope_follower_t<NV>
		this->getT(0).getT(0).getT(1).setExternalData(b, index);                         // Dynamics_impl::envelope_follower1_t<NV>
		this->getT(0).getT(1).getT(1).getT(0).getT(0).getT(1).setExternalData(b, index); // Dynamics_impl::comp2_t
		this->getT(0).getT(1).getT(1).getT(0).getT(1).getT(2).setExternalData(b, index); // Dynamics_impl::comp1_t
		this->getT(0).getT(1).getT(1).getT(0).getT(2).getT(2).setExternalData(b, index); // Dynamics_impl::comp3_t
		this->getT(0).getT(1).getT(1).getT(0).getT(3).getT(2).setExternalData(b, index); // Dynamics_impl::comp4_t
		this->getT(0).getT(1).getT(1).getT(0).getT(4).getT(2).setExternalData(b, index); // Dynamics_impl::comp5_t
		this->getT(0).getT(1).getT(1).getT(0).getT(5).getT(1).setExternalData(b, index); // Dynamics_impl::comp7_t
		this->getT(0).getT(2).getT(0).setExternalData(b, index);                         // Dynamics_impl::comp8_t
		this->getT(0).getT(4).getT(0).setExternalData(b, index);                         // Dynamics_impl::comp_t
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
using Dynamics = wrap::node<Dynamics_impl::instance<NV>>;
}


