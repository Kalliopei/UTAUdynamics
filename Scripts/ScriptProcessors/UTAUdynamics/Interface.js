Content.makeFrontInterface(332, 550);
Engine.loadFontAs("{PROJECT_FOLDER}Fonts/Px437_Portfolio_6x8.ttf", "Portfolio_6x8");

const var PeakIn = Synth.getEffect("PeakIn");
const var PeakOut = Synth.getEffect("PeakOut");
const var Gate = Synth.getEffect("Gate");
const var HPF = Synth.getEffect("HPF");
const var Dynamics = Synth.getEffect("Dynamics");

const var inputPanel = Content.addPanel("pnlInput", 130, 111);
const var outputPanel = Content.addPanel("pnlOutput", 275, 111);

const var MAX_PEAK_HOLD_FRAMES = 45;
const var MAX_PEAK_DECAY_RATE = 0.5;

Content.setPropertiesFromJSON("pnlInput", {
    "width": 15,
    "height": 333,
    "opaque": false
});

Content.setPropertiesFromJSON("pnlOutput", {
    "width": 15,
    "height": 333,
    "opaque": false
});

inputPanel.data.currentLevel = -60.0;
inputPanel.data.maxPeakdB = -60.0;
inputPanel.data.holdCounter = 0;

outputPanel.data.currentLevel = -60.0;
outputPanel.data.maxPeakdB = -60.0;
outputPanel.data.holdCounter = 0;

inline function dbToY(db, panelHeight)
{
    local clamped = Math.range(db, -60.0, 6.0);
    local percent = (clamped - (-60.0)) / (6.0 - (-60.0));
    return panelHeight - (percent * panelHeight);
};

inputPanel.setPaintRoutine(function(g)
{
    local w = this.getWidth();
    local h = this.getHeight();

    local floor = Content.getComponent("knbFloor").getValue();
    local slack = Content.getComponent("knbSlack").getValue();
    local fade = Content.getComponent("knbFade").getValue();
    local slackLevel = floor + slack;
    local fadeInLevel = floor + fade * 9;
    local fadeOutLevel = slackLevel - fade * 9;
    
    local rawLevel = PeakIn.getCurrentLevel(1);
    local LeveldB = (rawLevel > 0) ? Engine.getDecibelsForGainFactor(rawLevel) : -60.0;
    
    local peak = dbToY(LeveldB, h);
    local maxPeakY = dbToY(this.data.maxPeakdB, h);
    local floorY = dbToY(floor, h);
    local slackY = dbToY(slackLevel, h);
    local fadeInY = dbToY(fadeInLevel, h);
    local fadeOutY = dbToY(fadeOutLevel, h);
    
	g.setColour(0xFF433151);
	g.fillRect([3, maxPeakY - 1, 9, 2]);
	
	g.setColour(0xFFE5007F);
	g.fillRect([3, peak, 9, h - peak]);
    
    g.setColour(Colours.withAlpha(Colours.green, 0.2));
    g.fillRect([0, floorY, w, slackY - floorY]);

    g.setColour(Colours.withAlpha(Colours.whitesmoke, 0.5));
	g.fillRect([0, fadeInY - 1, w, 2]);
	
    g.setColour(Colours.withAlpha(Colours.whitesmoke, 0.5));
	g.fillRect([0, fadeOutY - 1, w, 2]);
	
    g.setColour(Colours.withAlpha(Colours.red, 1));
    g.fillRect([0, slackY - 1, w, 2]);
    
    g.setColour(Colours.withAlpha(Colours.green, 1));
    g.fillRect([0, floorY - 1, w, 2]);
});

outputPanel.setPaintRoutine(function(g)
{
    local w = this.getWidth();
    local h = this.getHeight();
    
	local rawLevel = PeakOut.getCurrentLevel(1);
	local LeveldB = (rawLevel > 0) ? Engine.getDecibelsForGainFactor(rawLevel) : -60.0;
	
    local peak = dbToY(LeveldB, h);
    local maxPeakY = dbToY(this.data.maxPeakdB, h);
	
	g.setColour(0xFFE5007F);
	g.fillRect([3, maxPeakY - 1, 9, 2]);
		
	g.setColour(0xFF433151);
	g.fillRect([3, peak, 9, h - peak]);
});

inputPanel.setTimerCallback(function()
{
    local rawLevel = PeakIn.getCurrentLevel(1);
    local leveldB = (rawLevel > 0) ? Engine.getDecibelsForGainFactor(rawLevel) : -60.0;
    
    if (leveldB > this.data.maxPeakdB)
    {
	    this.data.maxPeakdB = leveldB;
	    this.data.holdCounter = 0;
    }
    else {
	    this.data.holdCounter += 1;
	    
	    if (this.data.holdCounter > MAX_PEAK_HOLD_FRAMES)
	    	this.data.maxPeakdB = Math.max(leveldB, this.data.maxPeakdB - MAX_PEAK_DECAY_RATE);
    }

    this.repaint();
});

outputPanel.setTimerCallback(function()
{
    local rawLevel = PeakOut.getCurrentLevel(1);
    local leveldB = (rawLevel > 0) ? Engine.getDecibelsForGainFactor(rawLevel) : -60.0;
    
    if (leveldB > this.data.maxPeakdB)
    {
	    this.data.maxPeakdB = leveldB;
	    this.data.holdCounter = 0;
    }
    else {
	    this.data.holdCounter += 1;
	    
	    if (this.data.holdCounter > MAX_PEAK_HOLD_FRAMES)
	    	this.data.maxPeakdB = Math.max(leveldB, this.data.maxPeakdB - MAX_PEAK_DECAY_RATE);
    }

    this.repaint();
});

inputPanel.startTimer(30);
outputPanel.startTimer(30);

const var allEffects = [Gate, HPF, Dynamics];
const var bypassKnob = Content.getComponent("btnBypass");

inline function onBypassToggle(component, value)
{
    for (fx in allEffects)
        fx.setBypassed(value);
};

bypassKnob.setControlCallback(onBypassToggle);

const var title = Content.getComponent("UTAU");
const var titleh = Content.getComponent("UTAUh");
const var readout = Content.getComponent("dynamics");
const var readouth = Content.getComponent("dynamicsh");

const var floorKnob = Content.getComponent("knbFloor");
const var slackKnob = Content.getComponent("knbSlack");
const var fadeKnob = Content.getComponent("knbFade");
const var lowcutKnob = Content.getComponent("knbLowCut");
const var amountKnob = Content.getComponent("knbAmount");
const var colorKnob = Content.getComponent("knbColor");
const var clampKnob = Content.getComponent("knbClamp");
const var boostKnob = Content.getComponent("knbBoost");

const var allKnobs = [floorKnob, slackKnob, fadeKnob, lowcutKnob, amountKnob, colorKnob, clampKnob, boostKnob];

const var IDLE_TIMER = Engine.createTimerObject();
IDLE_TIMER.setTimerCallback(function()
{
    title.set("text", "UTAU");
    titleh.set("text", "UTAU");
    readout.set("text", "dynamics");
    readouth.set("text", "dynamics");
});

inline function setTitle(text)
{
    title.set("text", text);
    titleh.set("text", text);
};

inline function setReadout(value, unit)
{
    local formatted = Engine.doubleToString(value, 2) + unit;
    readout.set("text", formatted);
    readouth.set("text", formatted);
    IDLE_TIMER.startTimer(1000);
};

const var knobConfig = [
    { id: "knbFloor",  module: Gate,     index: 0, title: "FLOR", unit: " dB" },
    { id: "knbSlack",  module: Gate,     index: 1, title: "SLCK", unit: " dB" },
    { id: "knbFade",   module: Gate,     index: 2, title: "FADE", unit: "" },
    { id: "knbLowCut", module: HPF,      index: 1, title: "LWCT", unit: " hz" },
    { id: "knbAmount", module: Dynamics, index: 0, title: "AMNT", unit: "" },
    { id: "knbColor",  module: Dynamics, index: 2, title: "CLOR", unit: "" },
    { id: "knbClamp",  module: Dynamics, index: 1, title: "CLMP", unit: "" },
    { id: "knbBoost",  module: Dynamics, index: 3, title: "BOST", unit: " dB" }
];

inline function findConfig(id)
{
    for (cfg in knobConfig)
    {
        if (cfg.id == id)
            return cfg;
    }
    return {};
};

inline function onKnobChange(component, value)
{
    local cfg = findConfig(component.get("id"));
    cfg.module.setAttribute(cfg.index, value);
    setTitle(cfg.title);
    setReadout(value, cfg.unit);
};

for (k in allKnobs)
{
    local mods = k.createModifiers();
    k.setModifiers(mods.TextInput, [mods.rightClick, mods.noKeyModifier]);
    k.setControlCallback(onKnobChange);
};function onNoteOn()
{
	
}
 function onNoteOff()
{
	
}
 function onController()
{
	
}
 function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 