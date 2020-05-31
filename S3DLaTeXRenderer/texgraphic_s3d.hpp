#pragma once
#include<Siv3D.hpp>
#include "config.h"
#include "common.h"
#include "graphic/graphic.h"

namespace tex
{
	class TeXFont_impl : public tex::Font
	{
	protected:
		int32 m_baseSize = 60;
	public:
		virtual void draw(const float& x, const float& y, const String& str, const Color& col) = 0;
	};

	//s3d::SDFFont
	class TeXFont_SDF_impl : public TeXFont_impl
	{
	private:
		s3d::FilePath m_path = U"";
		int m_style;
		float m_size;
		std::shared_ptr<s3d::SDFFont> m_font;
	public:
		TeXFont_SDF_impl(const s3d::String& file, float size)
		{
			m_path = file;
			m_size = size;
			m_font = std::make_shared<s3d::SDFFont>((int32)(m_size * m_baseSize), m_path);
		}

		TeXFont_SDF_impl(const s3d::String& name, int style, float size)
		{
			s3d::FontStyle s = FontStyle::Default;
			switch (style)
			{
			case PLAIN:
				s = s3d::FontStyle::Default;
				break;
			case BOLD:
				s = s3d::FontStyle::Bold;
				break;
			case ITALIC:
				s = s3d::FontStyle::Italic;
				break;
			case BOLDITALIC:
				s = s3d::FontStyle::BoldItalic;
				break;
			}
			m_style = style;
			m_font = std::make_shared<s3d::SDFFont>((int32)(m_size * m_baseSize), Typeface::Default, s);
		}

		virtual float getSize() const override
		{
			return m_size;
		}

		/**
		 * Derive font from current font with given style
		 *
		 * @param style
		 *      required style, defined in enum TypefaceStyle,
		 *      we use integer type to represents style here
		 */
		virtual sptr<tex::Font> deriveFont(int style) const override
		{
			if (m_path.isEmpty())
			{
				return std::make_shared<TeXFont_SDF_impl>(U"", m_style, getSize());
			}
			else
			{
				return std::make_shared<TeXFont_SDF_impl>(m_path, getSize());
			}
		}

		/**
		 * Check if current font equals another
		 */
		virtual bool operator==(const tex::Font& f) const override
		{
			const auto& f3 = static_cast<const TeXFont_SDF_impl&>(f);
			return m_font == f3.m_font;
		}

		/**
		 * Check if current font not equals another
		 */
		virtual bool operator!=(const tex::Font& f) const override
		{
			return !(*this == f);
		}

		void draw(const float& x, const float& y, const String& str, const Color& col) override
		{
			s3d::Graphics2D::SetSDFParameters(m_font->pixelRange());
			m_font->operator()(str).draw(1, x, y - m_font->ascent() / m_baseSize, col);
		}
	};
	//s3d::Font
	class TeXFont_Normal_impl : public TeXFont_impl
	{
	private:
		s3d::FilePath m_path = U"";
		int m_style;
		float m_size;
		std::shared_ptr<s3d::Font> m_font;
	public:
		TeXFont_Normal_impl(const s3d::String& file, float size)
		{
			m_path = file;
			m_size = size;
			m_font = std::make_shared<s3d::Font>((int32)(m_size * m_baseSize), m_path);
		}

		TeXFont_Normal_impl(const s3d::String& name, int style, float size)
		{
			s3d::FontStyle s = FontStyle::Default;
			switch (style)
			{
			case PLAIN:
				s = s3d::FontStyle::Default;
				break;
			case BOLD:
				s = s3d::FontStyle::Bold;
				break;
			case ITALIC:
				s = s3d::FontStyle::Italic;
				break;
			case BOLDITALIC:
				s = s3d::FontStyle::BoldItalic;
				break;
			}
			m_style = style;
			m_font = std::make_shared<s3d::Font>((int32)(m_size * m_baseSize), Typeface::Default, s);
		}

		virtual float getSize() const override
		{
			return m_size;
		}

		/**
		 * Derive font from current font with given style
		 *
		 * @param style
		 *      required style, defined in enum TypefaceStyle,
		 *      we use integer type to represents style here
		 */
		virtual sptr<tex::Font> deriveFont(int style) const override
		{
			if (m_path.isEmpty())
			{
				return std::make_shared<TeXFont_Normal_impl>(U"", m_style, getSize());
			}
			else
			{
				return std::make_shared<TeXFont_Normal_impl>(m_path, getSize());
			}
		}

		/**
		 * Check if current font equals another
		 */
		virtual bool operator==(const tex::Font& f) const override
		{
			const auto& f3 = static_cast<const TeXFont_Normal_impl&>(f);
			return m_font == f3.m_font;
		}

		/**
		 * Check if current font not equals another
		 */
		virtual bool operator!=(const tex::Font& f) const override
		{
			return !(*this == f);
		}

		void draw(const float& x, const float& y, const String& str, const Color& col) override
		{
			Transformer2D transform(Mat3x2::Scale(1.0 / m_baseSize));
			m_font->operator()(str).draw(x, y - m_font->ascent(), col);
		}

		s3d::Font& getFont()
		{
			return *m_font;
		}
	};

	Font* Font::create(const string& file, float size)
	{
		return new TeXFont_Normal_impl(Unicode::Widen(file), size);
	}

	sptr<Font> Font::_create(const string& name, int style, float size)
	{
		return std::make_shared<TeXFont_Normal_impl>(Unicode::Widen(name), style, size);
	}

	class TextLayout_impl : public tex::TextLayout
	{
	public:
		void getBounds(_out_ Rect& bounds) override
		{

		}

		void draw(Graphics2D& g2, float x, float y) override
		{

		}
	};

	sptr<TextLayout> TextLayout::create(const wstring& src, const sptr<Font>& font) {
		return std::make_shared<TextLayout_impl>();
	}

	class Graphics2D_s3d : public Graphics2D
	{
	private:

		s3d::Color m_color;

		Stroke m_stroke;

		LineStyle m_lineStyle;

		const tex::Font* m_font;

		s3d::Mat3x2 m_mat;

		double m_fontSize;

		s3d::Float2 m_scale;
	public:

		void setColor(color c) override
		{
			m_color.a = color_a(c);
			m_color.r = color_r(c);
			m_color.g = color_g(c);
			m_color.b = color_b(c);
		}

		color getColor() const override
		{
			return argb((int)m_color.a, (int)m_color.r, (int)m_color.g, (int)m_color.b);
		}

		void setStroke(const Stroke& s) override
		{
			m_stroke = s;
			switch (m_stroke.cap)
			{
			case CAP_BUTT:
				m_lineStyle.hasCap = false;
				break;
			case CAP_ROUND:
				m_lineStyle.hasCap = true;
				m_lineStyle.isRound = true;
				break;
			case CAP_SQUARE:
				m_lineStyle.hasCap = true;
				m_lineStyle.isRound = false;
				break;
			}
		}

		const Stroke& getStroke() const override
		{
			return m_stroke;
		}

		void setStrokeWidth(float w) override
		{
			m_stroke.lineWidth = w;
		}

		const Font* getFont() const override
		{
			return m_font;
		}

		void setFont(const Font* font) override
		{
			m_font = font;
		}

		void translate(float dx, float dy) override
		{
			//Print << U"translate({},{})"_fmt(dx, dy);
			m_mat = Mat3x2::Translate(dx, dy) * m_mat;
		}

		void scale(float sx, float sy) override
		{
			//Print << U"scale({},{})"_fmt(sx, sy);
			m_mat = Mat3x2::Scale(sx, sy) * m_mat;
			m_scale *= {sx, sy};
		}

		void rotate(float angle) override
		{
			//Print << U"rotate({})"_fmt(angle);
			m_mat = Mat3x2::Rotate(angle) * m_mat;
		}

		void rotate(float angle, float px, float py) override
		{
			//Print << U"rotate({},{},{})"_fmt(angle, px, py);
			m_mat = Mat3x2::Rotate(angle, { px, py }) * m_mat;
		}

		void reset() override
		{
			//Print << U"reset()";
			m_mat = Mat3x2::Identity();
			m_fontSize = 1;
			m_scale.x = 1;
			m_scale.y = 1;
		}

		float sx() const override
		{
			//Print << U"sx()";
			return m_scale.x;
		}

		float sy() const override
		{
			//Print << U"sy()";
			return m_scale.y;
		}

		void drawChar(wchar_t c, float x, float y) override
		{
			wchar_t str[]{ c, L'\0' };
			drawText(str, x, y);
		}

		void drawText(const wstring& c, float x, float y) override
		{
			//Print << U"drawText(\"{}\",{},{})"_fmt(str, x, y);
			{
				Transformer2D transform(m_mat);
				((TeXFont_impl*)m_font)->draw(x, y, Unicode::FromWString(c), m_color);
			}
		}

		void drawLine(float x1, float y1, float x2, float y2) override
		{
			//Print << U"drawLine({},{},{},{})"_fmt(x1, y1, x2, y2);
			{
				//Print << m_mat;
				Transformer2D transform(m_mat);
				Line(x1, y1, x2, y2).draw(m_lineStyle, m_stroke.lineWidth, m_color);
			}
		}

		void drawRect(float x, float y, float w, float h) override
		{
			//Print << U"drawRect({},{},{},{})"_fmt(x, y, w, h);
			{
				Transformer2D transform(m_mat);
				RectF(x, y, w, h).drawFrame(m_stroke.lineWidth, m_color);
			}
		}

		void fillRect(float x, float y, float w, float h) override
		{
			//Print << U"fillRect({},{},{},{})"_fmt(x, y, w, h);
			{
				Transformer2D transform(m_mat);
				RectF(x, y, w, h).draw(m_color);
			}
		}

		void drawRoundRect(float x, float y, float w, float h, float rx, float ry) override
		{
			//Print << U"fillRoundRect({},{},{},{},{},{})"_fmt(x, y, w, h, rx, ry);
			{
				Transformer2D transform(m_mat);
				RoundRect(x, y, w, h, Min(rx, ry)).drawFrame(m_stroke.lineWidth, m_color);
			}
		}

		void fillRoundRect(float x, float y, float w, float h, float rx, float ry) override
		{
			//Print << U"fillRoundRect({},{},{},{},{},{})"_fmt(x, y, w, h, rx, ry);
			{
				Transformer2D transform(m_mat);
				RoundRect(x, y, w, h, Min(rx, ry)).draw(m_color);
			}
		}
	};
}