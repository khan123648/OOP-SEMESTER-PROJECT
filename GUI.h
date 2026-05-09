#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using namespace std;

// ═══════════════════════════════════════════════════════════════════════════
// COLOR PALETTE  —  Warm Peach / Rose Medical Theme
// ═══════════════════════════════════════════════════════════════════════════
namespace Theme {
    // Backgrounds
    const sf::Color BG{ 20,  14,  18 };       // Very dark warm black
    const sf::Color PANEL{ 32,  22,  26 };       // Dark rose-tinted panel
    const sf::Color PANEL2{ 44,  30,  36 };       // Mid panel
    const sf::Color PANEL3{ 58,  40,  48 };       // Hovered panel
    const sf::Color BORDER{ 90,  56,  68 };       // Muted rose border
    // Accent — Peach / Apricot
    const sf::Color BORDER_LIT{ 255, 178, 140 };   // Lit peach border
    const sf::Color ACCENT{ 255, 178, 140 };   // Main peach accent
    const sf::Color ACCENT_DIM{ 200, 110,  80 };   // Dimmer peach
    const sf::Color ACCENT_DARK{ 90,  38,  28 };   // Very dark peach bg
    // Semantic
    const sf::Color SUCCESS{ 80, 220, 160 };   // Mint green
    const sf::Color SUCCESS_DK{ 16,  70,  50 };
    const sf::Color WARNING{ 255, 210,  80 };   // Warm yellow
    const sf::Color WARNING_DK{ 90,  66,  10 };
    const sf::Color DANGER{ 255, 100, 110 };   // Soft red
    const sf::Color DANGER_DK{ 90,  22,  28 };
    // Text
    const sf::Color TEXT{ 255, 240, 230 };   // Warm cream
    const sf::Color TEXT_DIM{ 190, 148, 138 };   // Muted rose-cream
    const sf::Color TEXT_MUTED{ 90,  60,  70 };
    // Inputs
    const sf::Color INPUT_BG{ 24,  16,  20 };
    const sf::Color INPUT_FOCUS{ 36,  20,  28 };
    // Legacy alias
    const sf::Color HOVER{ 58,  40,  48 };

    // ── Peach gradient helper (top-bar decorative stripe) ──────────────────
    // These are used in drawTopBar for the shimmer line
    const sf::Color PEACH_LIGHT{ 255, 210, 185 };
    const sf::Color PEACH_DEEP{ 220,  90,  70 };
    const sf::Color ROSE_MID{ 255, 140, 120 };
}

// ═══════════════════════════════════════════════════════════════════════════
// GLOBAL FONT
// ═══════════════════════════════════════════════════════════════════════════
namespace GUIFont {
    inline const sf::Font* ptr = nullptr;
    inline void set(const sf::Font& f) { ptr = &f; }
    inline const sf::Font& get() {
        static sf::Font fallback;
        return ptr ? *ptr : fallback;
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// BUTTON
// ═══════════════════════════════════════════════════════════════════════════
class Button {
public:
    sf::RectangleShape shape;
    sf::Text           label{ GUIFont::get(), "", 13 };
    sf::Color          normalColor = Theme::PANEL2;
    sf::Color          hoverColor = Theme::PANEL3;
    sf::Color          borderNorm = Theme::BORDER;
    bool               isHovered = false;
    bool               enabled = true;

    // Left-accent glow bar (shown on hover)
    sf::RectangleShape accentBar;

    void init(const sf::Font& font,
        const string& text,
        float x, float y, float w, float h,
        sf::Color bg = Theme::PANEL2,
        sf::Color hover = Theme::PANEL3,
        sf::Color tc = Theme::TEXT,
        unsigned int csize = 13,
        sf::Color border = Theme::BORDER)
    {
        normalColor = bg;
        hoverColor = hover;
        borderNorm = border;

        shape.setPosition({ x, y });
        shape.setSize({ w, h });
        shape.setFillColor(bg);
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(border);

        label = sf::Text(font, text, csize);
        label.setFillColor(tc);
        _center(x, y, w, h);

        // Accent bar — left edge glow strip
        accentBar.setSize({ 3.f, h - 8.f });
        accentBar.setFillColor(Theme::ACCENT);
        accentBar.setPosition({ x + 1.f, y + 4.f });
    }

    void _center(float x, float y, float w, float h) {
        sf::FloatRect tb = label.getLocalBounds();
        label.setPosition({
            x + (w - tb.size.x) / 2.f - tb.position.x,
            y + (h - tb.size.y) / 2.f - tb.position.y
            });
    }

    bool contains(sf::Vector2f p) const {
        return shape.getGlobalBounds().contains(p);
    }

    void handleHover(sf::Vector2f mp) {
        isHovered = enabled && contains(mp);
        if (enabled) shape.setFillColor(isHovered ? hoverColor : normalColor);
    }

    void draw(sf::RenderWindow& win) {
        if (!enabled) {
            shape.setFillColor(Theme::PANEL);
            shape.setOutlineColor(Theme::BORDER);
            label.setFillColor(Theme::TEXT_MUTED);
        }
        else {
            shape.setOutlineColor(isHovered ? Theme::BORDER_LIT : borderNorm);
        }
        win.draw(shape);

        // Draw left accent bar on hover for a premium feel
        if (isHovered && enabled) {
            sf::Vector2f pos = shape.getPosition();
            sf::Vector2f sz = shape.getSize();
            accentBar.setPosition({ pos.x + 1.f, pos.y + 4.f });
            accentBar.setSize({ 3.f, sz.y - 8.f });
            win.draw(accentBar);
        }

        win.draw(label);
    }

    void moveTo(float x, float y) {
        shape.setPosition({ x, y });
        sf::Vector2f sz = shape.getSize();
        _center(x, y, sz.x, sz.y);
        accentBar.setPosition({ x + 1.f, y + 4.f });
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// TEXT BOX
// ═══════════════════════════════════════════════════════════════════════════
class TextBox {
public:
    sf::RectangleShape shape;
    sf::RectangleShape accentBar;
    sf::Text           display{ GUIFont::get(), "", 13 };
    sf::RectangleShape cursor;
    string             value;
    string             placeholder;
    bool               focused = false;
    bool               isPassword = false;
    int                maxLen = 64;
    float              bx = 0, by = 0, bw = 0, bh = 0;
    sf::Clock          cursorClock;

    void init(const sf::Font& font, const string& ph,
        float x, float y, float w, float h,
        bool pwd = false, unsigned int cs = 13)
    {
        bx = x; by = y; bw = w; bh = h;
        placeholder = ph;
        isPassword = pwd;

        shape.setPosition({ x, y });
        shape.setSize({ w, h });
        shape.setFillColor(Theme::INPUT_BG);
        shape.setOutlineThickness(1.f);
        shape.setOutlineColor(Theme::BORDER);

        accentBar.setSize({ 3.f, h - 8.f });
        accentBar.setFillColor(Theme::ACCENT);
        accentBar.setPosition({ x + 1.f, y + 4.f });

        display = sf::Text(font, ph, cs);
        display.setFillColor(Theme::TEXT_MUTED);
        display.setPosition({ x + 14.f, y + (h - (float)cs) / 2.f });

        cursor.setSize({ 1.5f, h - 14.f });
        cursor.setFillColor(Theme::ACCENT);
        cursor.setPosition({ x + 14.f, y + 7.f });
    }

    void handleEvent(const sf::Event& event) {
        if (!focused) return;
        if (const auto* te = event.getIf<sf::Event::TextEntered>()) {
            uint32_t c = te->unicode;
            if (c == 8) { if (!value.empty()) value.pop_back(); }
            else if (c >= 32 && c < 127 && (int)value.size() < maxLen)
                value += static_cast<char>(c);
        }
    }

    void handleClick(sf::Vector2f mouse) {
        focused = shape.getGlobalBounds().contains(mouse);
        shape.setFillColor(focused ? Theme::INPUT_FOCUS : Theme::INPUT_BG);
        shape.setOutlineColor(focused ? Theme::ACCENT_DIM : Theme::BORDER);
    }

    void draw(sf::RenderWindow& win) {
        win.draw(shape);
        if (focused) win.draw(accentBar);

        if (value.empty()) {
            display.setString(placeholder);
            display.setFillColor(Theme::TEXT_MUTED);
        }
        else {
            display.setString(isPassword ? string(value.size(), '*') : value);
            display.setFillColor(Theme::TEXT);
        }
        win.draw(display);

        if (focused) {
            float el = cursorClock.getElapsedTime().asSeconds();
            if (el > 1.f) cursorClock.restart();
            if (el < 0.5f) {
                sf::FloatRect tb = display.getLocalBounds();
                float cx = value.empty()
                    ? bx + 14.f
                    : bx + 14.f + tb.size.x + tb.position.x + 2.f;
                cursor.setPosition({ cx, by + 7.f });
                win.draw(cursor);
            }
        }
    }

    void clear() {
        value.clear();
        focused = false;
        shape.setFillColor(Theme::INPUT_BG);
        shape.setOutlineColor(Theme::BORDER);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// TOAST
// ═══════════════════════════════════════════════════════════════════════════
class Toast {
public:
    sf::RectangleShape bg;
    sf::RectangleShape sideBar;
    sf::Text           msg{ GUIFont::get(), "", 13 };
    sf::Clock          timer;
    bool               active = false;
    float              duration = 3.f;

    void init(const sf::Font& font, float winW, float winH) {
        bg.setSize({ 520.f, 46.f });
        bg.setPosition({ (winW - 520.f) / 2.f, winH - 66.f });
        bg.setFillColor(Theme::PANEL2);
        bg.setOutlineThickness(1.f);
        bg.setOutlineColor(Theme::BORDER);

        sideBar.setSize({ 4.f, 46.f });
        sideBar.setPosition({ (winW - 520.f) / 2.f, winH - 66.f });

        msg = sf::Text(font, "", 13);
        msg.setFillColor(Theme::TEXT);
    }

    void show(const string& text, sf::Color color = Theme::SUCCESS) {
        msg.setString(text);
        msg.setFillColor(color);
        bg.setOutlineColor(color);
        sideBar.setFillColor(color);
        sf::Vector2f bp = bg.getPosition();
        sf::FloatRect tb = msg.getLocalBounds();
        msg.setPosition({
            bp.x + 16.f,
            bp.y + (46.f - tb.size.y) / 2.f - tb.position.y
            });
        active = true;
        timer.restart();
    }

    void show(const string& text, bool success) {
        show(text, success ? Theme::SUCCESS : Theme::DANGER);
    }

    void draw(sf::RenderWindow& win) {
        if (!active) return;
        if (timer.getElapsedTime().asSeconds() > duration) { active = false; return; }
        win.draw(bg);
        win.draw(sideBar);
        win.draw(msg);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// SCROLL LIST
// ═══════════════════════════════════════════════════════════════════════════
class ScrollList {
public:
    vector<string>     items;
    sf::RectangleShape bg;
    sf::RectangleShape scrollBar;
    sf::RectangleShape scrollThumb;
    float              lx = 0, ly = 0, lw = 0, lh = 0;
    float              itemH = 36.f;
    float              scroll = 0.f;
    int                selected = -1;
    const sf::Font* fptr = nullptr;
    unsigned int       csize = 13;

    void init(const sf::Font& font, float x, float y, float w, float h, float ih = 36.f) {
        fptr = &font; lx = x; ly = y; lw = w; lh = h; itemH = ih;

        bg.setPosition({ x, y });
        bg.setSize({ w, h });
        bg.setFillColor(Theme::PANEL);
        bg.setOutlineThickness(1.f);
        bg.setOutlineColor(Theme::BORDER);

        scrollBar.setPosition({ x + w - 6.f, y });
        scrollBar.setSize({ 6.f, h });
        scrollBar.setFillColor(Theme::PANEL2);

        scrollThumb.setSize({ 6.f, 40.f });
        scrollThumb.setFillColor(Theme::ACCENT);
    }

    void setItems(const vector<string>& v) { items = v; scroll = 0.f; selected = -1; }

    void handleScroll(float delta) {
        float maxS = max(0.f, (float)items.size() * itemH - lh);
        scroll = max(0.f, min(scroll - delta * 28.f, maxS));
    }

    void handleClick(sf::Vector2f mouse) {
        if (!bg.getGlobalBounds().contains(mouse)) return;
        int idx = (int)((mouse.y - ly + scroll) / itemH);
        if (idx >= 0 && idx < (int)items.size()) selected = idx;
    }

    string getSelected() const {
        if (selected >= 0 && selected < (int)items.size()) return items[selected];
        return "";
    }

    void draw(sf::RenderWindow& win) {
        win.draw(bg);

        auto ws = win.getSize();
        sf::View clip;
        clip.setViewport({
            { lx / (float)ws.x, ly / (float)ws.y },
            { lw / (float)ws.x, lh / (float)ws.y }
            });
        clip.setSize({ lw, lh });
        clip.setCenter({ lw / 2.f, lh / 2.f + scroll });
        win.setView(clip);

        for (int i = 0; i < (int)items.size(); i++) {
            sf::RectangleShape row({ lw - 6.f, itemH - 1.f });
            row.setPosition({ 0.f, i * itemH });
            // Peach-tinted selection
            row.setFillColor(i == selected ? Theme::ACCENT_DARK
                : i % 2 == 0 ? Theme::PANEL
                : Theme::PANEL2);
            win.draw(row);

            if (i == selected) {
                sf::RectangleShape sel({ 3.f, itemH - 1.f });
                sel.setPosition({ 0.f, i * itemH });
                sel.setFillColor(Theme::ACCENT);
                win.draw(sel);
            }

            sf::Text t(*fptr, items[i], csize);
            t.setFillColor(i == selected ? Theme::ACCENT : Theme::TEXT);
            t.setPosition({ 14.f, i * itemH + (itemH - (float)csize) / 2.f - 1.f });
            win.draw(t);
        }

        win.setView(win.getDefaultView());

        float totalH = (float)items.size() * itemH;
        if (totalH > lh) {
            float thumbH = max(lh * lh / totalH, 20.f);
            float thumbY = ly + (totalH > lh ? scroll / (totalH - lh) * (lh - thumbH) : 0.f);
            scrollThumb.setSize({ 6.f, thumbH });
            scrollThumb.setPosition({ lx + lw - 6.f, thumbY });
            win.draw(scrollBar);
            win.draw(scrollThumb);
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// FREE HELPER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

inline sf::RectangleShape makePanel(float x, float y, float w, float h,
    sf::Color fill = Theme::PANEL,
    sf::Color border = Theme::BORDER,
    float     thickness = 1.f)
{
    sf::RectangleShape p;
    p.setPosition({ x, y });
    p.setSize({ w, h });
    p.setFillColor(fill);
    p.setOutlineThickness(thickness);
    p.setOutlineColor(border);
    return p;
}

inline sf::Text makeCentered(const sf::Font& font, const string& str,
    float cx, float cy,
    sf::Color color = Theme::TEXT, unsigned int sz = 16)
{
    sf::Text t(font, str, sz);
    t.setFillColor(color);
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition({ cx - b.size.x / 2.f - b.position.x,
                    cy - b.size.y / 2.f - b.position.y });
    return t;
}

inline sf::Text makeText(const sf::Font& font, const string& str,
    float x, float y,
    sf::Color color = Theme::TEXT,
    unsigned int sz = 13)
{
    sf::Text t(font, str, sz);
    t.setFillColor(color);
    t.setPosition({ x, y });
    return t;
}