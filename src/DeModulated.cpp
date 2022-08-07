#include "plugin.hpp"

//#define PI 3.141592653589793
#define TAU 2.*M_PI // Make life easier

#define DEG_TO_RAD 0.017453292519943 // For phase offset

// will be used later for optimization once brute force implementation is done
//using simd::float_4;

// Generate a point along a circle with
// center point x,y, radius R, angle theta
Vec circlePoint(float x, float y, float radius, float angle){
    Vec result(x,y);

    result.x += radius * sin(angle);
    result.y -= radius * cos(angle);
    return result;

}

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

    //float_4 phases[4];
    //float deltaPhase[16];
    float phases[16];
    float deltaPhase = 0;

	DeModulated() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		configParam(RATE_PARAM, 0.f, 20000.f, 1.f, "Rate", " hz");
		configSwitch(ACCUMULATE_PARAM, 0.f, 1.f, 0.f, "Accumulate",{"Off","On"});
		configParam(FMAMT_PARAM, -1.f, 1.f, 0.f, "Frequency Modulation", " %", 0.f, 100.f);
        configParam(OFFSET_PARAM, -360., 360., 360./16., "Phase Offset", " deg");
        // Param for attenuating/amplifying Offset Input amount
        configParam(OFFSETAMT_PARAM, -1.f, 1.f, 0.f, "Offset Depth", " %", 0.f, 100.f);

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
	
        float rateParam = params[RATE_PARAM].getValue();
        bool accumulateParam = params[ACCUMULATE_PARAM].getValue();
        float fmAmtParam = params[FMAMT_PARAM].getValue();
        float offsetAmtParam = params[OFFSETAMT_PARAM].getValue();
        

		int channels = 16;
        int numIn = inputs[PHASE_INPUT].getChannels();
        int numFM = inputs[FM_INPUT].getChannels();
        //int numOff = inputs[OFFSET_INPUT].isMonophonic

       

        for (int c = 0;c < channels; c++){
            
            if(accumulateParam){ // Internal accumulator is enabled
         
                deltaPhase = (rateParam)*args.sampleTime; 

                phases[c] += deltaPhase;
                phases[c] -= std::floor(phases[c]);
       
                if (outputs[POLYPHASE_OUTPUT].isConnected()) // Only output accumulator if cable is attached
                    //outputs[POLYPHASE_OUTPUT].setVoltage((std::sin((phases[c]+((params[OFFSET_PARAM].getValue()*DEG_TO_RAD)*c))+inputs[OFFSET_INPUT].getVoltage(c))*5.), c);

                    outputs[POLYPHASE_OUTPUT].setVoltage(std::sin(phases[c]*TAU+(((params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD))+(inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))))*5.,c);
                
                if (outputs[c+1].isConnected()) // Same as above but iterating through mono outs
                    //outputs[c+1].setVoltage((std::sin((phases[c]+((params[OFFSET_PARAM].getValue()*DEG_TO_RAD)*c))+inputs[OFFSET_INPUT].getVoltage(c))*5.), c);
                    //outputs[c+1].setVoltage((std::sin(phases[c]*TAU+(params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD))+inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))*5.,c);
                    //outputs[c+1].setVoltage(std::sin(phases[c]*TAU+(((params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD))+(inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))))*5.,c);
                    outputs[c+1].setVoltage(std::sin((phases[c]*TAU)+(params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD))+(inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))*5.);

            }
            else{ // Internal accumulator is disabled

                
                if (outputs[POLYPHASE_OUTPUT].isConnected())
                    if (inputs[PHASE_INPUT].getChannels()==1)
                        outputs[POLYPHASE_OUTPUT].setVoltage(std::sin((inputs[PHASE_INPUT].getVoltage()*TAU)+(params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD))+(inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))*5.,c);
                    else
                        outputs[POLYPHASE_OUTPUT].setVoltage(std::sin((inputs[PHASE_INPUT].getVoltage(c)*TAU)+(params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD))+(inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))*5., c);

                if (outputs[c+1].isConnected())
                    if (inputs[PHASE_INPUT].getChannels()==1)
                        outputs[c+1].setVoltage(std::sin((inputs[PHASE_INPUT].getVoltage()*TAU)+params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD)+(inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))*5.);
                    else
                        outputs[c+1].setVoltage(std::sin((inputs[PHASE_INPUT].getVoltage(c)*TAU)+(params[OFFSET_PARAM].getValue()*(c*DEG_TO_RAD))+(inputs[OFFSET_INPUT].getVoltage(c)*offsetAmtParam))*5.);

                        
            }

        }
			

		// Set Polyphase to output all channels
        outputs[POLYPHASE_OUTPUT].setChannels(channels-1);
        
        // Need to macro this at some point
        // Set each output to a single channel
        outputs[PHASE0_OUTPUT].setChannels(1);
        outputs[PHASE1_OUTPUT].setChannels(1);
        outputs[PHASE2_OUTPUT].setChannels(1);
        outputs[PHASE3_OUTPUT].setChannels(1);
        outputs[PHASE4_OUTPUT].setChannels(1);
        outputs[PHASE5_OUTPUT].setChannels(1);
        outputs[PHASE6_OUTPUT].setChannels(1);
        outputs[PHASE7_OUTPUT].setChannels(1);
        outputs[PHASE8_OUTPUT].setChannels(1);
        outputs[PHASE9_OUTPUT].setChannels(1);
        outputs[PHASE10_OUTPUT].setChannels(1);
        outputs[PHASE11_OUTPUT].setChannels(1);
        outputs[PHASE12_OUTPUT].setChannels(1);
        outputs[PHASE13_OUTPUT].setChannels(1);
        outputs[PHASE14_OUTPUT].setChannels(1);
		outputs[PHASE15_OUTPUT].setChannels(1);
        
		}
        

	};



struct DeModulatedWidget : ModuleWidget {
	DeModulatedWidget(DeModulated* module) {

		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/DeModulated.svg")));
        

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        
        
        addInput(createInputCentered<PJ301MPort>(mm2px(circlePoint(60.96/2.,40.,10.,0.*DEG_TO_RAD)), module, DeModulated::FM_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(circlePoint(60.96/2.,40.,10.,180.*DEG_TO_RAD)), module, DeModulated::PHASE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(circlePoint(60.96/2.,40.,10.,270.*DEG_TO_RAD)), module, DeModulated::OFFSET_INPUT));
		
        addParam(createParamCentered<VCVLatch>(mm2px(Vec(10.,10.)), module, DeModulated::ACCUMULATE_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20.,10.)), module, DeModulated::RATE_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.,10.)), module, DeModulated::FMAMT_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(40.,10.)), module, DeModulated::OFFSET_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(50.,10.)), module, DeModulated::OFFSETAMT_PARAM));

        const float offset = ((360./16.))*DEG_TO_RAD;
        const float radius = 25.;
        Vec circleCentre(60.96/2.,90.);

		addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,0.,0.)), module, DeModulated::POLYPHASE_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,0.)), module, DeModulated::PHASE0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset)), module, DeModulated::PHASE1_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*2)), module, DeModulated::PHASE2_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*3)), module, DeModulated::PHASE3_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*4)), module, DeModulated::PHASE4_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*5)), module, DeModulated::PHASE5_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*6)), module, DeModulated::PHASE6_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*7)), module, DeModulated::PHASE7_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*8)), module, DeModulated::PHASE8_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*9)), module, DeModulated::PHASE9_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*10)), module, DeModulated::PHASE10_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*11)), module, DeModulated::PHASE11_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*12)), module, DeModulated::PHASE12_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*13)), module, DeModulated::PHASE13_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*14)), module, DeModulated::PHASE14_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(circlePoint(circleCentre.x,circleCentre.y,radius,offset*15)), module, DeModulated::PHASE15_OUTPUT));
        
	}
};


Model* modelDeModulated = createModel<DeModulated, DeModulatedWidget>("DeModulated");
