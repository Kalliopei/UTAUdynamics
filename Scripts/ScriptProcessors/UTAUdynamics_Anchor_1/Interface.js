Content.makeFrontInterface(332, 550);

Engine.loadFontAs("{PROJECT_FOLDER}Fonts/Px437_Portfolio_6x8.ttf", "Portfolio_6x8");



const InputMeter = Synth.getEffect("InputMeter");
const OutputMeter = Synth.getEffect("OutputMeter");

const HPF = Synth.getEffect("HPF");
const Gate = Synth.getEffect("Gate");
const Dynamics = Synth.getEffect("Dynamics");

const PreComp = Synth.getEffect("PreComp");
const PostComp = Synth.getEffect("PostComp");
const Makeup = Synth.getEffect("Makeup");



const InputPanel = Content.addPanel("InputPanel");
const OutputPanel = Content.addPanel("OutputPanel");
const ReductionPanel = Content.addPanel("ReductionPanel");



const Effects1 = [HPF, Gate];
const Effects2 = [PreComp, PostComp, Dynamics];



const TitleLabel = Content.getComponent("TitleLabel");
const TitleLabelH = Content.getComponent("TitleLabelH");
const ReadoutLabel = Content.getComponent("ReadoutLabel");
const ReadoutLabelH = Content.getComponent("ReadoutLabelH");
const Effects1BypassButton = Content.getComponent("Effects1BypassButton");
const Effects2BypassButton = Content.getComponent("Effects2BypassButton");
const FloorKnob = Content.getComponent("FloorKnob");
const SlackKnob = Content.getComponent("SlackKnob");
const FadeKnob = Content.getComponent("FadeKnob");
const LowCutKnob = Content.getComponent("LowCutKnob");
const AmountKnob = Content.getComponent("AmountKnob");
const ColorKnob = Content.getComponent("ColorKnob");
const ClampKnob = Content.getComponent("ClampKnob");
const BoostKnob = Content.getComponent("BoostKnob");

const AllKnobs = [FloorKnob, SlackKnob, FadeKnob, LowCutKnob, AmountKnob, ColorKnob, ClampKnob, BoostKnob];



const KnobConfig = [
    { id: "FloorKnob",  targets: [{ module: Gate, index: 0 }],      title: "FLOR", unit: " dB" },
    { id: "SlackKnob",  targets: [{ module: Gate, index: 1 }],      title: "SLCK", unit: " dB" },
    { id: "FadeKnob",   targets: [{ module: Gate, index: 2 }],      title: "FADE", unit: "" },
    { id: "LowCutKnob", targets: [{ module: HPF, index: 1 }],       title: "LWCT", unit: " hz" },
    { id: "AmountKnob", targets: [{ module: Dynamics, index: 0 }, { module: Makeup, index: 0 }],  title: "AMNT", unit: "" },
    { id: "ColorKnob",  targets: [{ module: Dynamics, index: 1 }],  title: "COLR", unit: "" },
    { id: "ClampKnob",  targets: [{ module: Dynamics, index: 2 }],  title: "CLMP", unit: "" },
    { id: "BoostKnob",  targets: [{ module: Makeup, index: 1 }],    title: "BOST", unit: " dB" }
];



const MAX_PEAK_HOLD_FRAMES = 45;
const MAX_PEAK_DECAY_RATE = 0.5;
const IDLE_TIMER = Engine.createTimerObject();



Content.setPropertiesFromJSON("InputPanel", {
    "x": 113,
    "y": 99,
    "width": 15,
    "height": 321,
    "opaque": false
});

Content.setPropertiesFromJSON("OutputPanel", {
    "x": 269,
    "y": 99,
    "width": 15,
    "height": 321,
    "opaque": false
});

Content.setPropertiesFromJSON("ReductionPanel", {
    "x": 269,
    "y": 99,
    "width": 15,
    "height": 321,
    "opaque": false
});



InputPanel.data.CurrentLevel = -60.0;
InputPanel.data.MaxPeakDb = -60.0;
InputPanel.data.HoldCount = 0;

OutputPanel.data.CurrentLevel = -60.0;
OutputPanel.data.MaxPeakDb = -60.0;
OutputPanel.data.HoldCount = 0;

ReductionPanel.data.CurrentReduction = 0.0;



inline function ReductionToY(reduction, height)
{
    var Clamped = Math.range(reduction, 0.0, 60.0);
    var Percent = Clamped / 60;
    return Percent * height;
};

inline function DecibelsToY(dB, height)
{
    var Clamped = Math.range(dB, -60.0, 0.0);
    var Percent = (Clamped - (-60.0)) / (0.0 - (-60.0));
    return height - (Percent * height);
};



InputPanel.setPaintRoutine(function(draw)
{
    var Width = this.getWidth();
    var Height = this.getHeight();

    var Floor = Content.getComponent("FloorKnob").getValue();
    var Slack = Content.getComponent("SlackKnob").getValue();
    var Fade = Content.getComponent("FadeKnob").getValue();

    var SlackLevel = Floor + Slack;
    var FadeInLevel = Floor + Fade * 9;
    var FadeOutLevel = SlackLevel - Fade * 9;
    var RawLevel = InputMeter.getCurrentLevel(1);
    var LevelDb = (RawLevel > 0) ? Engine.getDecibelsForGainFactor(RawLevel) : -60.0;

    var Peak = DecibelsToY(LevelDb, Height);
    var MaxPeakY = DecibelsToY(this.data.MaxPeakDb, Height);
    var FloorY = DecibelsToY(Floor, Height);
    var SlackY = DecibelsToY(SlackLevel, Height);
    var FadeInY = DecibelsToY(FadeInLevel, Height);
    var FadeOutY = DecibelsToY(FadeOutLevel, Height);



    draw.setColour(Colours.withAlpha(Colours.whitesmoke, 0.9));
    draw.fillRect([3, MaxPeakY - 1, 9, 2]);

    draw.setColour(Colours.withAlpha(Colours.whitesmoke, 0.7));
    draw.fillRect([3, Peak, 9, Height - Peak]);

    draw.setColour(Colours.withAlpha(0xFFE5007F, 0.5));
    draw.fillRect([0, FloorY, Width, SlackY - FloorY]);

    draw.setColour(Colours.withAlpha(Colours.whitesmoke, 0.5));
    draw.fillRect([0, FadeInY - 1, Width, 2]);

    draw.setColour(Colours.withAlpha(Colours.whitesmoke, 0.5));
    draw.fillRect([0, FadeOutY - 1, Width, 2]);

    draw.setColour(Colours.withAlpha(0xFFE5007F, 1));
    draw.fillRect([0, SlackY - 1, Width, 2]);

    draw.setColour(Colours.withAlpha(0xFFE5007F, 1));
    draw.fillRect([0, FloorY - 1, Width, 2]);
});

OutputPanel.setPaintRoutine(function(draw)
{
    var Width = this.getWidth();
    var Height = this.getHeight();

    var RawLevel = OutputMeter.getCurrentLevel(1);
    var LevelDb = (RawLevel > 0) ? Engine.getDecibelsForGainFactor(RawLevel) : -60.0;

    var Peak = DecibelsToY(LevelDb, Height);
    var MaxPeakY = DecibelsToY(this.data.MaxPeakDb, Height);


    
    draw.setColour(Colours.withAlpha(Colours.whitesmoke, 0.9));
    draw.fillRect([3, MaxPeakY - 1, 9, 2]);

    draw.setColour(Colours.withAlpha(Colours.whitesmoke, 0.7));
    draw.fillRect([3, Peak, 9, Height - Peak]);
});

ReductionPanel.setPaintRoutine(function(draw)
{
    var Width = this.getWidth();
    var Height = this.getHeight();

    var ReductionY = ReductionToY(this.data.CurrentReduction, Height);

    draw.setColour(Colours.withAlpha(0xFF433151, 0.9));
    draw.fillRect([0, ReductionY - 1, Width, 2]);
});



InputPanel.setTimerCallback(function()
{
    var RawLevel = InputMeter.getCurrentLevel(1);
    var LevelDb = (RawLevel > 0) ? Engine.getDecibelsForGainFactor(RawLevel) : -60.0;

    if (LevelDb > this.data.MaxPeakDb)
    {
        this.data.MaxPeakDb = LevelDb;
        this.data.HoldCount = 0;
    }
    else {
        this.data.HoldCount += 1;

        if (this.data.HoldCount > MAX_PEAK_HOLD_FRAMES)
            this.data.MaxPeakDb = Math.max(LevelDb, this.data.MaxPeakDb - MAX_PEAK_DECAY_RATE);
    }

    this.repaint();
});

OutputPanel.setTimerCallback(function()
{
    var RawLevel = OutputMeter.getCurrentLevel(1);
    var LevelDb = (RawLevel > 0) ? Engine.getDecibelsForGainFactor(RawLevel) : -60.0;

    if (LevelDb > this.data.MaxPeakDb)
    {
        this.data.MaxPeakDb = LevelDb;
        this.data.HoldCount = 0;
    }
    else {
        this.data.HoldCount += 1;

        if (this.data.HoldCount > MAX_PEAK_HOLD_FRAMES)
            this.data.MaxPeakDb = Math.max(LevelDb, this.data.MaxPeakDb - MAX_PEAK_DECAY_RATE);
    }

    this.repaint();
});

ReductionPanel.setTimerCallback(function()
{
    var PreDb = (PreComp.getCurrentLevel(1) > 0) ? Engine.getDecibelsForGainFactor(PreComp.getCurrentLevel(1)) : -60.0;
    var PostDb = (PostComp.getCurrentLevel(1) > 0) ? Engine.getDecibelsForGainFactor(PostComp.getCurrentLevel(1)) : -60.0;

    this.data.CurrentReduction = Math.max(0.0, PreDb - PostDb);
    this.repaint();
});



InputPanel.startTimer(30);
OutputPanel.startTimer(30);
ReductionPanel.startTimer(30);



inline function Effects1BypassToggle(component, value)
{
    for (fx in Effects1)
        fx.setBypassed(value);
};

inline function Effects2BypassToggle(component, value)
{
    for (fx in Effects2)
        fx.setBypassed(value);
};

Effects1BypassButton.setControlCallback(Effects1BypassToggle);
Effects2BypassButton.setControlCallback(Effects2BypassToggle);



IDLE_TIMER.setTimerCallback(function()
{
    TitleLabel.set("text", "UTAU");
    TitleLabelH.set("text", "UTAU");
    ReadoutLabel.set("text", "dynamics");
    ReadoutLabelH.set("text", "dynamics");
});



inline function SetTitle(text)
{
    TitleLabel.set("text", text);
    TitleLabelH.set("text", text);
};

inline function SetReadout(value, unit)
{
    var Formatted = Engine.doubleToString(value, 2) + unit;
    ReadoutLabel.set("text", Formatted);
    ReadoutLabelH.set("text", Formatted);
    IDLE_TIMER.startTimer(3000);
};

inline function FindConfig(id)
{
    for (Config in KnobConfig)
    {
        if (Config.id == id)
            return Config;
    }
    return {};
};

inline function KnobChangeCallback(component, value)
{
    var Config = FindConfig(component.get("id"));

    for (Table in Config.targets)
        Table.module.setAttribute(Table.index, value);

    SetTitle(Config.title);
    SetReadout(value, Config.unit);
};

for (Knob in AllKnobs)
{
    var Modifiers = Knob.createModifiers();
    Knob.setModifiers(Modifiers.TextInput, [Modifiers.rightClick, Modifiers.noKeyModifier]);
    Knob.setControlCallback(KnobChangeCallback);
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
 