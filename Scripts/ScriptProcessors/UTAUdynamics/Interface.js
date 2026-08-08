Content.makeFrontInterface(335, 550);

Engine.loadFontAs("{PROJECT_FOLDER}Fonts/Px437_Portfolio_6x8.ttf", "Portfolio_6x8");

const var meterPanel = Content.addPanel("MeterPanel", 122, 116);
const var Gate = Synth.getEffect("Gate");
const var HPF = Synth.getEffect("HPF");
const var Dynamics = Synth.getEffect("Dynamics");

Content.setPropertiesFromJSON("MeterPanel", {
    "width": 15,
    "height": 210,
    "opaque": false
});

meterPanel.data.currentLevel = -60.0;

inline function dbToY(db, panelHeight)
{
    var clamped = Math.range(db, -60.0, 6.0);
    var percent = (clamped - (-60.0)) / (6.0 - (-60.0));
    return panelHeight - (percent * panelHeight);
};

meterPanel.setPaintRoutine(function(g)
{
	var w = this.getWidth();
	var h = this.getHeight();

	var floor = Content.getComponent("knbFloor").getValue();
	var slack = Content.getComponent("knbSlack").getValue();
	
	var floorLevel = floor;
	var slackLevel = floor + slack;
	
	var floorY = dbToY(floorLevel, h);
	var slackY = dbToY(slackLevel, h);
	
	g.setColour(Colours.withAlpha(Colours.whitesmoke, 0.25));
	g.fillRect([0, floorY, w, slackY - floorY]);
	
	g.setColour(Colours.withAlpha(Colours.whitesmoke, 1));
	g.fillRect([0, floorY - 1, w, 2]);
	
	g.setColour(Colours.withAlpha(Colours.whitesmoke, 0.5));
	g.fillRect([0, slackY - 1, w, 2]);
});

meterPanel.setTimerCallback(function()
{
    this.data.currentLevel = Gate.getAttribute(0);
    this.repaint();
});

meterPanel.startTimer(30);

const var title = Content.getComponent("UTAU");
const var titleh = Content.getComponent("UTAUh");
const var readout = Content.getComponent("dynamics");
const var readouth = Content.getComponent("dynamicsh");

const var floorKnob = Content.getComponent("knbFloor");
const var slackKnob = Content.getComponent("knbSlack");
const var speedKnob = Content.getComponent("knbSpeed");
const var lowcutKnob = Content.getComponent("knbLowCut");
const var amountKnob = Content.getComponent("knbAmount");
const var ratioKnob = Content.getComponent("knbRatio");
const var tightKnob = Content.getComponent("knbTight");
const var boostKnob = Content.getComponent("knbBoost");

const var allKnobs = [floorKnob, slackKnob, speedKnob, lowcutKnob, amountKnob, ratioKnob, tightKnob, boostKnob];
const var TIMER_VAR = Engine.createTimerObject();
TIMER_VAR.setTimerCallback(function()
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
}
inline function setReadout(component, value, text)
{
    readout.set("text", Engine.doubleToString(value, 2) + text);
    readouth.set("text", Engine.doubleToString(value, 2) + text);
    TIMER_VAR.startTimer(1000);
};

// 1. Config and helper functions FIRST
const var knobConfig = [
	{ id: "knbFloor",  module: Gate,     index: 0, title: "FLOR", unit: " dB" },
	{ id: "knbSlack",  module: Gate,     index: 1, title: "SLCK", unit: " dB" },
	{ id: "knbSpeed",  module: Gate,     index: 2, title: "SPED", unit: "" },
	{ id: "knbLowCut", module: HPF,      index: 1, title: "LWCT", unit: " hz" },
	{ id: "knbAmount", module: Dynamics, index: 0, title: "AMNT", unit: "" },
	{ id: "knbRatio",  module: Dynamics, index: 1, title: "RTIO", unit: "" },
	{ id: "knbTight",  module: Dynamics, index: 2, title: "TGHT", unit: "" },
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
	setReadout(component, value, cfg.unit);
};

// 2. THEN the loop that uses it
for (k in allKnobs)
{
    var mods = k.createModifiers();
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
 