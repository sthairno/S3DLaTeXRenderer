# include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include"SasaGUI.hpp"
#include"s3dLaTeX.h"

void Main()
{
	LaTeXRenderer::init();
	SasaGUI::GUIManager gui;

	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));

	LaTeXRenderer latex;

	String tex = U"\\int_{now}^{+\\infty} \\text{Keeptrying}";
	
	HSV texColor = Palette::Black;

	bool paramChanged = true;
	while (System::Update())
	{
		ClearPrint();
		gui.frameBegin();

		gui.groupBegin(U"", true);
		if (latex)
		{
			gui.callback([&](RectF rect)
				{
					latex.draw(rect.pos.asPoint(), texColor);
				}, latex.getSize());
		}
		gui.groupEnd();

		gui.windowBegin(U"cLaTeXMath", SasaGUI::AutoResize);
		{
			gui.label(U"Width"); gui.spinBox(latex.Width, 10.f, 1000.f); gui.newLine();
			gui.label(U"TextSize"); gui.spinBox(latex.TextSize, 10.f, 100.f); gui.newLine();
			gui.label(U"LineSpace"); gui.spinBox(latex.LineSpace, -100.f, 100.f); gui.newLine();
			gui.label(U"Align"); gui.dropdownList(latex.Align, { LaTeXAlign::Left,LaTeXAlign::Center,LaTeXAlign::Right }); gui.newLine();
			if (gui.treeNode(U"Color"))
			{
				gui.newLine();
				gui.colorPicker(texColor);
			}
			gui.newLine();
			if (gui.textBox(tex, U"", SasaGUI::All | SasaGUI::MultiLine, 300, 5))
			{
				latex.parse(tex);
			}
			gui.newLine();
			gui.groupBegin(U"Error", true);
			{
				if (latex.getErrorMsg())
				{
					gui.label(latex.getErrorMsg(), ColorF(Palette::Red));
				}
				else
				{
					gui.label(U"None");
				}
			}
			gui.groupEnd(); gui.newLine();
			gui.groupBegin(U"Size",true);
			{
				if (latex)
				{
					gui.label(Format(latex.getSize()));
				}
				else
				{
					gui.label(U"(---,---)");
				}
			}
			gui.groupEnd();
		}
		gui.windowEnd();
		gui.frameEnd();
	}
	LaTeXRenderer::release();
}