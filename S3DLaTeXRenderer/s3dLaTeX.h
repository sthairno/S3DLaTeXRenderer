#pragma once
#include<Siv3D.hpp>

namespace tex
{
	class TeXFormula;
	class TeXRender;
	class Graphics2D;
}

namespace s3d
{
	enum class LaTeXAlign
	{
		Left,
		Right,
		Center
	};

	constexpr StringView ToString(LaTeXAlign e)
	{
		switch (e)
		{
		case LaTeXAlign::Left:
			return U"Left";
		case LaTeXAlign::Right:
			return U"Right";
		case LaTeXAlign::Center:
			return U"Center";
		};
	}
	void Formatter(FormatData& formatData, const LaTeXAlign& value);

	class LaTeXRenderer
	{
	private:
		std::shared_ptr<tex::TeXFormula> m_formula;

		std::shared_ptr<tex::TeXRender> m_render;

		std::shared_ptr <tex::Graphics2D> m_graphic;

		String m_errorMsg;

		Color m_color;

		void generateTeXRender();
	public:
		/// <summary>
		/// 描画位置
		/// </summary>
		LaTeXAlign Align = LaTeXAlign::Left;

		/// <summary>
		/// 文字の大きさ
		/// </summary>
		float TextSize = 20;

		/// <summary>
		/// 行間
		/// </summary>
		float LineSpace = 2;
		
		/// <summary>
		/// 描画するときの横幅(AlignがRight,Centerの時のみ有効)
		/// </summary>
		float Width = 500;

		LaTeXRenderer();

		LaTeXRenderer(const String& code);

		Size getSize();

		String getErrorMsg();

		bool parse(const String& code);

		void draw(const Point& pos, const Color& color = Palette::White);

		void overwrite(Image& dst, const Point& pos, const Color& color = Palette::White);

		void paint(Image& dst, const Point& pos, const Color& color = Palette::White);
		
		[[nodiscard]] bool isParsed() const;

		[[nodiscard]] explicit operator bool() const
		{
			return isParsed();
		}

		static void init();
		static void release();
	};
}