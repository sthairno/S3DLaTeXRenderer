#include "s3dLaTeX.h"
#define GRAPHICS_DEBUG
#define HAVE_LOG
#include "texgraphic_s3d.hpp"
#include "latex.h"
#include "core/formula.h"

void s3d::Formatter(FormatData& formatData, const LaTeXAlign& value)
{
	formatData.string.append(ToString(value));
}

void s3d::LaTeXRenderer::generateTeXRender()
{
	tex::TeXRenderBuilder builder;
	int align = tex::ALIGN_LEFT;
	switch (Align)
	{
	case LaTeXAlign::Left:
		align = tex::ALIGN_LEFT;
		break;
	case LaTeXAlign::Right:
		align = tex::ALIGN_RIGHT;
		break;
	case LaTeXAlign::Center:
		align = tex::ALIGN_CENTER;
		break;
	}
	m_render = std::shared_ptr<tex::TeXRender>(builder
		.setStyle(tex::STYLE_DISPLAY)
		.setTextSize(TextSize)
		.setWidth(tex::UNIT_PIXEL, Width, align)
		.setIsMaxWidth(Align == LaTeXAlign::Left)
		.setLineSpace(tex::UNIT_PIXEL, LineSpace)
		.setForeground(tex::argb((int)m_color.a, (int)m_color.r, (int)m_color.g, (int)m_color.b))
		.build(*m_formula));
}

s3d::LaTeXRenderer::LaTeXRenderer()
{
	m_graphic = std::make_shared<tex::Graphics2D_s3d>();
}

s3d::LaTeXRenderer::LaTeXRenderer(const String& code)
{
	m_graphic = std::make_shared<tex::Graphics2D_s3d>();
	parse(code);
}

Size s3d::LaTeXRenderer::getSize()
{
	if (isParsed())
	{
		return Size(m_render->getWidth(), m_render->getHeight());
	}
	else
	{
		return Size(0, 0);
	}
}

String s3d::LaTeXRenderer::getErrorMsg()
{
	return m_errorMsg;
}

bool s3d::LaTeXRenderer::parse(const String& code)
{
	m_errorMsg = U"";
	try
	{
		m_formula = std::make_shared<tex::TeXFormula>(code.toWstr());
		generateTeXRender();
		return true;
	}
	catch (const tex::ex_tex& ex)
	{
		m_errorMsg = Unicode::Widen(ex.what());
		return false;
	}
}

void s3d::LaTeXRenderer::draw(const Point& pos, const Color& color)
{
	if (!isParsed())
	{
		return;
	}
	m_color = color;
	generateTeXRender();
	m_render->draw(*m_graphic, pos.x, pos.y);
}

void s3d::LaTeXRenderer::overwrite(Image& dst, const Point& pos, const Color& color)
{
	if (!isParsed())
	{
		return;
	}
}

void s3d::LaTeXRenderer::paint(Image& dst, const Point& pos, const Color& color)
{
	if (!isParsed())
	{
		return;
	}
}

bool s3d::LaTeXRenderer::isParsed() const
{
	return !!m_formula;
}

void s3d::LaTeXRenderer::init()
{
	tex::LaTeX::init("engine/cLaTeXMath/res");
}

void s3d::LaTeXRenderer::release()
{
	tex::LaTeX::release();
}