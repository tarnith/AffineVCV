#include "plugin.hpp"

#define PI 3.14159
#define TAU 2.*PI
using simd::float_4;


struct DeModulated : Module {
	enum ParamIds {
		RATE_PARAM,
        ACCUMULATE_PARAM,
        FMAMT_PARAM,
        OFFSET_PARAM,
        OFFSETAMT_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PHASE_INPUT,
        OFFSET_INPUT,
        FM_INPUT,
        NUM_INPUTS
	};
	enum OutputIds {
		POLYPHASE_OUTPUT,
        PHASE0_OUTPUT,
        PHASE1_OUTPUT,
        PHASE2_OUTPUT,
        PHASE3_OUTPUT,
        PHASE4_OUTPUT,
        PHASE5_OUTPUT,
        PHASE6_OUTPUT,
        PHASE7_OUTPUT,
        PHASE8_OUTPUT,
        PHASE9_OUTPUT,
        PHASE10_OUTPUT,
        PHASE11_OUTPUT,
        PHASE12_OUTPUT,
        PHASE13_OUTPUT,
        PHASE14_OUTPUT,
        PHASE15_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

    float_4 phases[4];


	DeModulated() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		configParam(RATE_PARAM, 0.f, 20000.f, 1.f, "Rate", " hz");
		configSwitch(ACCUMULATE_PARAM, 0.f, 1.f, 0.f, "Accumulate",{"Off","On"});
		configParam(FMAMT_PARAM, -1.f, 1.f, 0.f, "Frequency Modulation", "%", 0.f, 100.f);
        configParam(OFFSET_PARAM, -TAU, TAU, TAU/16., "Phase Offset", "rads");
        // Param for attenuating/amplifying Offset Input amount
        configParam(OFFSETAMT_PARAM, -1.f, 1.f, 0.f, "Offset Modulation", "%", 0.f, 100.f);

        configInput(PHASE_INPUT, "Phase");
        configInput(OFFSET_INPUT, "Offset");
        configInput(FM_INPUT, "FM");

        configOutput(POLYPHASE_OUTPUT, "PolyPhase Out");
        configOutput(PHASE0_OUTPUT, "Phase 0");
        configOutput(PHASE1_OUTPUT, "Phase 1");
        configOutput(PHASE2_OUTPUT, "Phase 2");
        configOutput(PHASE3_OUTPUT, "Phase 3");
        configOutput(PHASE4_OUTPUT, "Phase 4");
        configOutput(PHASE5_OUTPUT, "Phase 5");
        configOutput(PHASE6_OUTPUT, "Phase 6");
        configOutput(PHASE7_OUTPUT, "Phase 7");
        configOutput(PHASE8_OUTPUT, "Phase 8");
        configOutput(PHASE9_OUTPUT, "Phase 9");
        configOutput(PHASE10_OUTPUT, "Phase 10");
        configOutput(PHASE11_OUTPUT, "Phase 11");
        configOutput(PHASE12_OUTPUT, "Phase 12");
        configOutput(PHASE13_OUTPUT, "Phase 13");
        configOutput(PHASE14_OUTPUT, "Phase 14");
        configOutput(PHASE15_OUTPUT, "Phase 15");
        
	
	}

	void process(const ProcessArgs& args) override {
		// 0.16-0.19 us serial
		// 0.23 us serial with all lambdas computed
		// 0.15-0.18 us serial with all lambdas computed with SSE

		int channels = 16;

			

		
        outputs[POLYPHASE_OUTPUT].setchannels(channels);
        outputs[PHASE0_OUTPUT].setchannels(1);
        outputs[PHASE1_OUTPUT].setchannels(2);
        outputs[PHASE2_OUTPUT].setchannels(3);
        outputs[PHASE3_OUTPUT].setchannels(4);
        outputs[PHASE4_OUTPUT].setchannels(5);
        outputs[PHASE5_OUTPUT].setchannels(6);
        outputs[PHASE6_OUTPUT].setchannels(7);
        outputs[PHASE7_OUTPUT].setchannels(8);
        outputs[PHASE8_OUTPUT].setchannels(9);
        outputs[PHASE9_OUTPUT].setchannels(10);
        outputs[PHASE10_OUTPUT].setchannels(11);
        outputs[PHASE11_OUTPUT].setchannels(12);
        outputs[PHASE12_OUTPUT].setchannels(13);
        outputs[PHASE13_OUTPUT].setchannels(14);
        outputs[PHASE14_OUTPUT].setchannels(15);
		outputs[PHASE15_OUTPUT].setchannels(16);

		}
	}

	};



struct DeModulatedWidget : ModuleWidget {
	ADSRWidget(ADSR* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "BLANKPROVIDESVG!!!!!")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));


	}
};


Model* modelDeModulated = createModel<DeModulated, DeModulatedWidget>("DeModulated");
