#include "Background.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

const float GW = 800.0f;
const float GH = 720.0f;

// ─────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────
static void filledRect(float x, float y, float w, float h,
                        float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x,   y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y+h);
        glVertex2f(x,   y+h);
    glEnd();
}

static void filledRectA(float x, float y, float w, float h,
                         float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(x,   y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y+h);
        glVertex2f(x,   y+h);
    glEnd();
}

static void outlineRect(float x, float y, float w, float h,
                         float r, float g, float b, float lw = 1.5f)
{
    glColor3f(r, g, b);
    glLineWidth(lw);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x,   y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y+h);
        glVertex2f(x,   y+h);
    glEnd();
}

static void filledCircle(float cx, float cy, float r,
                          float R, float G, float B)
{
    glColor3f(R, G, B);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 60; i++) {
            float a = 2.0f * M_PI * i / 60;
            glVertex2f(cx + r*cos(a), cy + r*sin(a));
        }
    glEnd();
}

// Draw horizontal wood plank lines over a region
static void woodPlanks(float x, float y, float w, float h,
                        float plankH, float lineR, float lineG, float lineB)
{
    glColor3f(lineR, lineG, lineB);
    glLineWidth(1.0f);
    for (float py = y; py < y + h; py += plankH) {
        glBegin(GL_LINES);
            glVertex2f(x,   py);
            glVertex2f(x+w, py);
        glEnd();
    }
}

// Draw vertical plank lines
static void woodPlanksV(float x, float y, float w, float h,
                         float plankW, float lineR, float lineG, float lineB)
{
    glColor3f(lineR, lineG, lineB);
    glLineWidth(1.0f);
    for (float px = x; px < x + w; px += plankW) {
        glBegin(GL_LINES);
            glVertex2f(px, y);
            glVertex2f(px, y+h);
        glEnd();
    }
}

// ─────────────────────────────────────────────────────────────
// Sub-drawers
// ─────────────────────────────────────────────────────────────

// Outdoor scene visible through barn door
static void drawOutdoorScene(float x, float y, float w, float h)
{
    // Sky
    filledRect(x, y + h*0.35f, w, h*0.65f, 0.55f, 0.82f, 0.95f);
    // Grass
    filledRect(x, y, w, h*0.38f, 0.35f, 0.72f, 0.20f);

    // Simple clouds
    auto cloud = [&](float cx, float cy, float sc) {
        glColor3f(1,1,1);
        for (int i = 0; i < 3; i++) {
            float ox = (i-1) * 18.0f * sc;
            glBegin(GL_POLYGON);
            for (int j = 0; j <= 30; j++) {
                float a = 2*M_PI*j/30;
                glVertex2f(cx+ox + 14*sc*cos(a), cy + 9*sc*sin(a));
            }
            glEnd();
        }
    };
    cloud(x + w*0.30f, y + h*0.72f, 0.8f);
    cloud(x + w*0.65f, y + h*0.78f, 0.7f);
    cloud(x + w*0.85f, y + h*0.70f, 0.55f);
}

// Pendant light + light cone
static void drawPendantLight(float cx, float topY, float coneH)
{

    // Shade (trapezoid)
    float sw = 28, sh = 22, nt = 10;
    float shadeY = topY - 28 - sh;

       // Warm yellow glow at bottom of shade
    glColor3f(1.0f, 0.92f, 0.55f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, shadeY + sh);
        for (int i = 0; i <= 20; i++) {
            float a = M_PI * i / 20;
            glVertex2f(cx + 10*cos(a), shadeY + sh + 5*sin(a));
        }
    glEnd();

    // Light cone (translucent yellow)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float coneW = coneH * 0.75f;
    float coneTopY = shadeY + sh;

    glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 0.90f, 0.40f, 0.55f);
        glVertex2f(cx, coneTopY);
        for (int i = 0; i <= 40; i++) {
            float t = (float)i / 40.0f;
            float px = cx + (t - 0.5f) * 2.0f * coneW;
            float py = coneTopY - coneH * fabs(t - 0.5f) * 0.3f;
            (void)py;
            glColor4f(1.0f, 0.88f, 0.30f, 0.0f);
            glVertex2f(px, coneTopY - coneH);
        }
    glEnd();

    // Cone using proper triangle fan from apex
    glBegin(GL_TRIANGLE_FAN);
        glColor4f(1.0f, 0.92f, 0.50f, 0.50f);
        glVertex2f(cx, coneTopY);
        glColor4f(1.0f, 0.88f, 0.30f, 0.0f);
        glVertex2f(cx - coneW, coneTopY - coneH);
        glColor4f(1.0f, 0.88f, 0.30f, 0.02f);
        for (int i = 1; i <= 20; i++) {
            float t = (float)i / 20.0f;
            float px = cx - coneW + t * 2.0f * coneW;
            glVertex2f(px, coneTopY - coneH);
        }
        glColor4f(1.0f, 0.88f, 0.30f, 0.0f);
        glVertex2f(cx + coneW, coneTopY - coneH);
    glEnd();


    // Cord
    glColor3f(0.25f, 0.18f, 0.10f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(cx, topY + 5);
        glVertex2f(cx, topY - 28);
    glEnd();


    glColor3f(0.35f, 0.38f, 0.42f);
    glBegin(GL_QUADS);
        glVertex2f(cx - nt,  shadeY + sh);
        glVertex2f(cx + nt,  shadeY + sh);
        glVertex2f(cx + sw,  shadeY);
        glVertex2f(cx - sw,  shadeY);
    glEnd();
    // Shade highlight
    glColor3f(0.50f, 0.53f, 0.58f);
    glBegin(GL_LINES);
        glVertex2f(cx - nt, shadeY + sh);
        glVertex2f(cx + nt, shadeY + sh);
    glEnd();
    // Shade outline
    glColor3f(0.15f, 0.12f, 0.08f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(cx - nt,  shadeY + sh);
        glVertex2f(cx + nt,  shadeY + sh);
        glVertex2f(cx + sw,  shadeY);
        glVertex2f(cx - sw,  shadeY);
    glEnd();



    glDisable(GL_BLEND);
}

// Wooden barrel
static void drawBarrel(float cx, float y, float w, float h)
{
    // Body (dark brown)
    float hw = w * 0.5f;
    glColor3f(0.48f, 0.28f, 0.10f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 40; i++) {
        float a = M_PI * i / 40;
        // barrel is slightly wider in middle
        float bx = cx + hw * (1.0f + 0.12f * sin(a)) * cos(a - M_PI*0.5f);
        float by = y + h * (float)i / 40.0f;
        glVertex2f(bx, by);
    }
    glEnd();

    // Simpler: just draw ellipse body
    glColor3f(0.52f, 0.30f, 0.12f);
    glBegin(GL_POLYGON);
        for (int i = 180; i <= 360; i++) {
            float a = i * M_PI / 180.0f;
            glVertex2f(cx + hw*cos(a), y + h*0.08f + h*0.08f*sin(a));
        }
        for (int i = 0; i <= 180; i++) {
            float a = i * M_PI / 180.0f;
            glVertex2f(cx + hw*cos(a), y + h*0.92f + h*0.08f*sin(a));
        }
    glEnd();

    // Metal bands
    float bands[3] = { 0.20f, 0.50f, 0.80f };
    for (int b = 0; b < 3; b++) {
        float by = y + h * bands[b];
        glColor3f(0.55f, 0.55f, 0.58f);
        glBegin(GL_QUADS);
            glVertex2f(cx - hw - 2, by - 4);
            glVertex2f(cx + hw + 2, by - 4);
            glVertex2f(cx + hw + 2, by + 4);
            glVertex2f(cx - hw - 2, by + 4);
        glEnd();
        // Band outline
        glColor3f(0.35f, 0.35f, 0.38f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(cx - hw - 2, by - 4);
            glVertex2f(cx + hw + 2, by - 4);
            glVertex2f(cx + hw + 2, by + 4);
            glVertex2f(cx - hw - 2, by + 4);
        glEnd();
    }

    // Vertical plank lines on barrel
    glColor3f(0.38f, 0.20f, 0.06f);
    glLineWidth(1.0f);
    for (int p = -3; p <= 3; p++) {
        float px = cx + p * hw / 3.5f;
        glBegin(GL_LINES);
            glVertex2f(px, y + h*0.10f);
            glVertex2f(px, y + h*0.90f);
        glEnd();
    }

    // Top ellipse
    glColor3f(0.45f, 0.25f, 0.08f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 40; i++) {
        float a = 2*M_PI*i/40;
        glVertex2f(cx + hw*cos(a), y + h + h*0.06f*sin(a));
    }
    glEnd();
    outlineRect(cx - hw, y, w, h, 0.28f, 0.14f, 0.04f, 1.5f);
}

// Milk can - proper aluminium churn shape
static void drawMilkCan(float cx, float y, float w, float h)
{
    float hw = w * 0.5f;

    // Shadow under can
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f,0.0f,0.0f,0.18f);
    glBegin(GL_POLYGON);
    for(int i=0;i<=30;i++){float a=2*M_PI*i/30; glVertex2f(cx+hw*0.9f*cos(a), y+h*0.03f*sin(a));}
    glEnd();
    glDisable(GL_BLEND);

    // Body fill – medium grey
    glColor3f(0.68f, 0.70f, 0.73f);
    glBegin(GL_POLYGON);
        glVertex2f(cx - hw*0.60f, y + h);          // neck L
        glVertex2f(cx + hw*0.60f, y + h);          // neck R
        glVertex2f(cx + hw,       y + h*0.82f);    // shoulder R
        glVertex2f(cx + hw,       y + h*0.18f);    // body R
        glVertex2f(cx + hw*0.80f, y + h*0.04f);    // base R
        glVertex2f(cx - hw*0.80f, y + h*0.04f);    // base L
        glVertex2f(cx - hw,       y + h*0.18f);    // body L
        glVertex2f(cx - hw,       y + h*0.82f);    // shoulder L
    glEnd();

    // Left-side darker shading
    glColor3f(0.50f, 0.52f, 0.55f);
    glBegin(GL_POLYGON);
        glVertex2f(cx - hw,       y + h*0.18f);
        glVertex2f(cx - hw*0.55f, y + h*0.18f);
        glVertex2f(cx - hw*0.45f, y + h*0.82f);
        glVertex2f(cx - hw,       y + h*0.82f);
    glEnd();

    // Right-side highlight stripe
    glColor3f(0.88f, 0.90f, 0.93f);
    glBegin(GL_POLYGON);
        glVertex2f(cx + hw*0.15f, y + h*0.20f);
        glVertex2f(cx + hw*0.38f, y + h*0.20f);
        glVertex2f(cx + hw*0.35f, y + h*0.80f);
        glVertex2f(cx + hw*0.12f, y + h*0.80f);
    glEnd();

    // Neck ring band
    glColor3f(0.55f, 0.57f, 0.60f);
    glBegin(GL_QUADS);
        glVertex2f(cx - hw*0.62f, y + h*0.89f);
        glVertex2f(cx + hw*0.62f, y + h*0.89f);
        glVertex2f(cx + hw*0.62f, y + h*0.96f);
        glVertex2f(cx - hw*0.62f, y + h*0.96f);
    glEnd();

    // Lid – ellipse shape
    glColor3f(0.60f, 0.62f, 0.65f);
    glBegin(GL_POLYGON);
    for(int i=0;i<=30;i++){
        float a = M_PI*i/30;
        glVertex2f(cx + hw*0.68f*cos(a), y+h + hw*0.22f*sin(a));
    }
    glVertex2f(cx+hw*0.68f, y+h);
    glVertex2f(cx-hw*0.68f, y+h);
    glEnd();

    // Lid knob
    glColor3f(0.48f, 0.50f, 0.53f);
    glBegin(GL_POLYGON);
    for(int i=0;i<=20;i++){float a=2*M_PI*i/20; glVertex2f(cx+hw*0.12f*cos(a), y+h+hw*0.32f+hw*0.10f*sin(a));}
    glEnd();

    // Handle arc
    glColor3f(0.42f, 0.44f, 0.47f);
    glLineWidth(2.5f);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<=18;i++){
        float a=M_PI*i/18;
        glVertex2f(cx+hw*0.50f*cos(a), y+h+hw*0.32f+hw*0.38f*sin(a));
    }
    glEnd();

    // Outline
    glColor3f(0.35f, 0.37f, 0.40f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(cx - hw*0.60f, y + h);
        glVertex2f(cx + hw*0.60f, y + h);
        glVertex2f(cx + hw,       y + h*0.82f);
        glVertex2f(cx + hw,       y + h*0.18f);
        glVertex2f(cx + hw*0.80f, y + h*0.04f);
        glVertex2f(cx - hw*0.80f, y + h*0.04f);
        glVertex2f(cx - hw,       y + h*0.18f);
        glVertex2f(cx - hw,       y + h*0.82f);
    glEnd();
}

// Ceramic pot
static void drawPot(float cx, float y, float w, float h,
                     float r, float g, float b)
{
    float hw = w * 0.5f;
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex2f(cx - hw*0.45f, y + h);        // neck
        glVertex2f(cx + hw*0.45f, y + h);
        glVertex2f(cx + hw,       y + h*0.72f);  // shoulder
        glVertex2f(cx + hw*0.95f, y + h*0.35f);  // body bulge
        glVertex2f(cx + hw*0.60f, y);            // base
        glVertex2f(cx - hw*0.60f, y);
        glVertex2f(cx - hw*0.95f, y + h*0.35f);
        glVertex2f(cx - hw,       y + h*0.72f);
    glEnd();
    // Darker rim
    glColor3f(r*0.75f, g*0.75f, b*0.75f);
    glBegin(GL_QUADS);
        glVertex2f(cx - hw*0.5f, y + h - 5);
        glVertex2f(cx + hw*0.5f, y + h - 5);
        glVertex2f(cx + hw*0.5f, y + h);
        glVertex2f(cx - hw*0.5f, y + h);
    glEnd();
    // Highlight
    glColor3f(fminf(r+0.25f,1.0f), fminf(g+0.15f,1.0f), fminf(b+0.10f,1.0f));
    glBegin(GL_POLYGON);
        glVertex2f(cx - hw*0.15f, y + h*0.40f);
        glVertex2f(cx + hw*0.05f, y + h*0.40f);
        glVertex2f(cx + hw*0.05f, y + h*0.70f);
        glVertex2f(cx - hw*0.15f, y + h*0.70f);
    glEnd();
    // Outline
    glColor3f(r*0.55f, g*0.55f, b*0.55f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(cx - hw*0.45f, y + h);
        glVertex2f(cx + hw*0.45f, y + h);
        glVertex2f(cx + hw,       y + h*0.72f);
        glVertex2f(cx + hw*0.95f, y + h*0.35f);
        glVertex2f(cx + hw*0.60f, y);
        glVertex2f(cx - hw*0.60f, y);
        glVertex2f(cx - hw*0.95f, y + h*0.35f);
        glVertex2f(cx - hw,       y + h*0.72f);
    glEnd();
}

// Wire egg basket
static void drawEggBasket(float cx, float y, float w, float h)
{
    float hw = w*0.5f;
    // Basket body (wire look - grid of dark lines)
    // Base fill
    glColor3f(0.25f, 0.22f, 0.18f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 30; i++) {
        float a = M_PI * i / 30;
        glVertex2f(cx + hw*cos(a), y + h*0.55f + h*0.12f*sin(a));
    }
    glVertex2f(cx + hw*0.85f, y + h*0.10f);
    glVertex2f(cx - hw*0.85f, y + h*0.10f);
    glEnd();

    // Wire grid lines horizontal
    glColor3f(0.18f, 0.15f, 0.10f);
    glLineWidth(1.5f);
    for (int row = 0; row < 4; row++) {
        float ry = y + h*0.12f + row * h*0.12f;
        float rw = hw * (0.85f + 0.05f * sin((float)row / 3 * M_PI));
        glBegin(GL_LINES);
            glVertex2f(cx - rw, ry);
            glVertex2f(cx + rw, ry);
        glEnd();
    }
    // Wire grid lines vertical
    for (int col = -3; col <= 3; col++) {
        glBegin(GL_LINES);
            glVertex2f(cx + col * hw/3.2f, y + h*0.10f);
            glVertex2f(cx + col * hw/3.2f, y + h*0.55f);
        glEnd();
    }

    // Arched handle
    glColor3f(0.18f, 0.15f, 0.10f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 20; i++) {
        float a = M_PI * i / 20;
        glVertex2f(cx + hw*0.80f*cos(a), y + h*0.55f + h*0.55f*sin(a));
    }
    glEnd();

    // Eggs inside basket
    float eggColors[3][3] = {
        {0.96f, 0.93f, 0.82f},
        {0.98f, 0.95f, 0.85f},
        {0.94f, 0.90f, 0.78f},
    };
    int eggCount = 6;
    for (int e = 0; e < eggCount; e++) {
        float ex = cx - hw*0.55f + e * (hw * 1.1f / (eggCount-1));
        float ey = y + h*0.25f + (e%2) * h*0.08f;
        int ci = e % 3;
        glColor3f(eggColors[ci][0], eggColors[ci][1], eggColors[ci][2]);
        glBegin(GL_POLYGON);
        for (int j = 0; j <= 20; j++) {
            float a = 2*M_PI*j/20;
            float stretch = (sin(a) < 0) ? 1.1f : 1.0f;
            glVertex2f(ex + hw*0.12f*cos(a)*0.8f*stretch,
                       ey + hw*0.16f*sin(a)*stretch);
        }
        glEnd();
    }
}

// Wooden ladder
static void drawLadder(float x, float y, float w, float h)
{
    float railW = w * 0.12f;
    // Left rail
    glColor3f(0.52f, 0.30f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(x,        y);
        glVertex2f(x+railW,  y);
        glVertex2f(x+railW,  y+h);
        glVertex2f(x,        y+h);
    glEnd();
    // Right rail
    glBegin(GL_QUADS);
        glVertex2f(x+w-railW, y);
        glVertex2f(x+w,       y);
        glVertex2f(x+w,       y+h);
        glVertex2f(x+w-railW, y+h);
    glEnd();
    // Rungs
    glColor3f(0.45f, 0.25f, 0.08f);
    int rungs = 8;
    for (int r = 0; r < rungs; r++) {
        float ry = y + h * (float)r / (rungs - 1);
        glBegin(GL_QUADS);
            glVertex2f(x+railW,   ry - 3);
            glVertex2f(x+w-railW, ry - 3);
            glVertex2f(x+w-railW, ry + 3);
            glVertex2f(x+railW,   ry + 3);
        glEnd();
    }
    // Outline
    glColor3f(0.28f, 0.14f, 0.04f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y); glVertex2f(x+w, y);
        glVertex2f(x+w, y+h); glVertex2f(x, y+h);
    glEnd();
}

// ─────────────────────────────────────────────────────────────
// Background constructor / update
// ─────────────────────────────────────────────────────────────
Background::Background()
{
    cloudOffset = cloudOffset1 = cloudOffset2 = cloudOffset3 = 0.0f;
    sunAngle = timeOfDay = skySpeed = windTime = 0.0f;
}

void Background::update() {}   // static barn scene – nothing to animate

// ─────────────────────────────────────────────────────────────
// Main draw
// ─────────────────────────────────────────────────────────────
void Background::draw(int W, int H)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ── Coordinate notes ──────────────────────────────────────
    // OpenGL origin at bottom-left.  H = screen height.
    // All Y values expressed from bottom (0) to top (H).
    // ─────────────────────────────────────────────────────────

    const float WALL_TOP  = (float)H;          // 720
    const float FLOOR_TOP = H * 0.28f;         // ~202  floor/wall boundary
    const float LOFT_Y    = H * 0.52f;         // ~374  loft platform bottom
    const float LOFT_TOP  = LOFT_Y + H*0.06f;  // ~417  loft platform top
    const float CEIL_PEAK = H * 0.92f;         // ~663  ceiling ridge
    const float CX        = W * 0.50f;         // 400   horizontal center

    // ═══════════════════════════════════════════════════════════
    // 1.  BACK WALL  (warm brown wood)
    // ═══════════════════════════════════════════════════════════
    filledRect(0, FLOOR_TOP, (float)W, WALL_TOP - FLOOR_TOP,
               0.72f, 0.48f, 0.22f);

    // Horizontal plank lines on back wall
    woodPlanks(0, FLOOR_TOP, (float)W, WALL_TOP - FLOOR_TOP,
               22.0f, 0.58f, 0.36f, 0.14f);

    // ═══════════════════════════════════════════════════════════
    // 2.  VAULTED CEILING  (two angled panels meeting at ridge)
    // ═══════════════════════════════════════════════════════════
    // Left ceiling panel
    glColor3f(0.80f, 0.58f, 0.28f);
    glBegin(GL_POLYGON);
        glVertex2f(0,    H * 0.62f);
        glVertex2f(0,    WALL_TOP);
        glVertex2f(CX,   WALL_TOP);
        glVertex2f(CX,   CEIL_PEAK);
    glEnd();

    // Right ceiling panel
    glBegin(GL_POLYGON);
        glVertex2f(CX,   CEIL_PEAK);
        glVertex2f(CX,   WALL_TOP);
        glVertex2f((float)W, WALL_TOP);
        glVertex2f((float)W, H * 0.62f);
    glEnd();

    // Ceiling plank lines (diagonal feel – horizontal lines on angled quad)
    glColor3f(0.65f, 0.44f, 0.18f);
    glLineWidth(1.2f);
    for (int p = 0; p < 12; p++) {
        float t = (float)p / 12.0f;
        // Left panel lines
        float ly1 = H*0.62f + t*(WALL_TOP - H*0.62f);
        float ly2 = CEIL_PEAK + t*(WALL_TOP - CEIL_PEAK);
        glBegin(GL_LINES);
            glVertex2f(0,  ly1);
            glVertex2f(CX, ly2);
        glEnd();
        // Right panel lines
        float ry1 = CEIL_PEAK + t*(WALL_TOP - CEIL_PEAK);
        float ry2 = H*0.62f + t*(WALL_TOP - H*0.62f);
        glBegin(GL_LINES);
            glVertex2f(CX,       ry1);
            glVertex2f((float)W, ry2);
        glEnd();
    }

    // ═══════════════════════════════════════════════════════════
    // 3.  MAIN HORIZONTAL BEAM  (red-circled in reference image)
    //     A thick dark-wood beam running full width near top
    // ═══════════════════════════════════════════════════════════
    float beamY  = H * 0.74f;
    float beamH  = H * 0.045f;
    // Beam fill – dark warm wood
    glColor3f(0.42f, 0.25f, 0.08f);
    glBegin(GL_QUADS);
        glVertex2f(0,        beamY);
        glVertex2f((float)W, beamY);
        glVertex2f((float)W, beamY + beamH);
        glVertex2f(0,        beamY + beamH);
    glEnd();
    // Top highlight
    glColor3f(0.55f, 0.35f, 0.14f);
    glBegin(GL_QUADS);
        glVertex2f(0,        beamY + beamH - 4);
        glVertex2f((float)W, beamY + beamH - 4);
        glVertex2f((float)W, beamY + beamH);
        glVertex2f(0,        beamY + beamH);
    glEnd();
    // Bottom shadow
    glColor3f(0.28f, 0.15f, 0.04f);
    glBegin(GL_QUADS);
        glVertex2f(0,        beamY);
        glVertex2f((float)W, beamY);
        glVertex2f((float)W, beamY + 4);
        glVertex2f(0,        beamY + 4);
    glEnd();
    // Outline
    glColor3f(0.20f, 0.10f, 0.02f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0,        beamY);
        glVertex2f((float)W, beamY);
        glVertex2f((float)W, beamY + beamH);
        glVertex2f(0,        beamY + beamH);
    glEnd();
    // Plank texture on beam
    woodPlanksV(0, beamY, (float)W, beamH, 55.0f, 0.32f, 0.18f, 0.05f);

    // ═══════════════════════════════════════════════════════════
    // 4.  SIDE WALLS  (left and right, converging perspective)
    // ═══════════════════════════════════════════════════════════
    // Left wall - subtle vertical planks
    glColor3f(0.65f, 0.42f, 0.18f);
    glBegin(GL_POLYGON);
        glVertex2f(0,          FLOOR_TOP);
        glVertex2f(W * 0.22f,  FLOOR_TOP);
        glVertex2f(W * 0.22f,  H * 0.80f);
        glVertex2f(0,          WALL_TOP);
    glEnd();
    glColor3f(0.50f, 0.30f, 0.10f);
    glLineWidth(1.0f);
    for(float px = 0; px < W*0.22f; px += 26.0f){
        glBegin(GL_LINES);
            glVertex2f(px, FLOOR_TOP);
            glVertex2f(px * 0.5f, WALL_TOP);
        glEnd();
    }

    // Right wall - subtle vertical planks, no heavy horizontal lines
    glColor3f(0.62f, 0.40f, 0.16f);
    glBegin(GL_POLYGON);
        glVertex2f(W * 0.78f,  FLOOR_TOP);
        glVertex2f((float)W,   FLOOR_TOP);
        glVertex2f((float)W,   WALL_TOP);
        glVertex2f(W * 0.78f,  H * 0.80f);
    glEnd();
    // Very subtle vertical plank lines only (no horizontal)
    glColor3f(0.52f, 0.32f, 0.10f);
    glLineWidth(1.0f);
    for(float px = W*0.80f; px < (float)W; px += 28.0f){
        glBegin(GL_LINES);
            glVertex2f(px, FLOOR_TOP);
            glVertex2f(px + 8, H*0.85f);
        glEnd();
    }

    // ═══════════════════════════════════════════════════════════
    // 5.  FLOOR  (wider planks, perspective lines converge to door)
    // ═══════════════════════════════════════════════════════════
    filledRect(0, 0, (float)W, FLOOR_TOP, 0.52f, 0.33f, 0.13f);

    // Subtle tile grid on floor - horizontal lines only, very faint
    glColor3f(0.42f, 0.25f, 0.08f);
    glLineWidth(1.0f);
    int tileRows = 6;
    for(int r = 1; r < tileRows; r++){
        float ty = FLOOR_TOP * r / tileRows;
        glBegin(GL_LINES);
            glVertex2f(0, ty);
            glVertex2f((float)W, ty);
        glEnd();
    }
    // Vertical tile lines - converge slightly toward center
    for(int c = 0; c <= 8; c++){
        float bx = (float)W * c / 8.0f;
        float tx = W*0.5f + (bx - W*0.5f)*0.35f;
        glBegin(GL_LINES);
            glVertex2f(bx, 0);
            glVertex2f(tx, FLOOR_TOP);
        glEnd();
    }

    // ═══════════════════════════════════════════════════════════
    // 6.  BARN DOOR  (central open door with outdoor scene)
    // ═══════════════════════════════════════════════════════════
    float doorW = W * 0.28f;
    float doorH = H * 0.30f;
    float doorX = CX - doorW * 0.5f;
    float doorY = FLOOR_TOP - doorH * 0.02f;

    // Draw outdoor scene inside door
    drawOutdoorScene(doorX, doorY, doorW, doorH);

    // Door frame (thick dark wood border)
    float frameT = 10.0f;
    glColor3f(0.38f, 0.22f, 0.07f);
    // Top frame
    glBegin(GL_QUADS);
        glVertex2f(doorX - frameT,         doorY + doorH);
        glVertex2f(doorX + doorW + frameT, doorY + doorH);
        glVertex2f(doorX + doorW + frameT, doorY + doorH + frameT);
        glVertex2f(doorX - frameT,         doorY + doorH + frameT);
    glEnd();
    // Left frame
    glBegin(GL_QUADS);
        glVertex2f(doorX - frameT, doorY - frameT);
        glVertex2f(doorX,          doorY - frameT);
        glVertex2f(doorX,          doorY + doorH + frameT);
        glVertex2f(doorX - frameT, doorY + doorH + frameT);
    glEnd();
    // Right frame
    glBegin(GL_QUADS);
        glVertex2f(doorX + doorW,          doorY - frameT);
        glVertex2f(doorX + doorW + frameT, doorY - frameT);
        glVertex2f(doorX + doorW + frameT, doorY + doorH + frameT);
        glVertex2f(doorX + doorW,          doorY + doorH + frameT);
    glEnd();

    // Door curtains / open sides (darkened wood)
    glColor3f(0.44f, 0.26f, 0.09f);
    glBegin(GL_QUADS);
        glVertex2f(doorX,             doorY);
        glVertex2f(doorX + doorW*0.18f, doorY);
        glVertex2f(doorX + doorW*0.18f, doorY + doorH);
        glVertex2f(doorX,             doorY + doorH);
    glEnd();
    glBegin(GL_QUADS);
        glVertex2f(doorX + doorW*0.82f, doorY);
        glVertex2f(doorX + doorW,       doorY);
        glVertex2f(doorX + doorW,       doorY + doorH);
        glVertex2f(doorX + doorW*0.82f, doorY + doorH);
    glEnd();

    // ═══════════════════════════════════════════════════════════
    // 7.  LOFT PLATFORM  (mid-height shelf across center)
    // ═══════════════════════════════════════════════════════════
    float loftX  = W * 0.20f;
    float loftW  = W * 0.60f;
    float loftThick = H * 0.030f;

    // Loft top surface
    glColor3f(0.55f, 0.34f, 0.12f);
    glBegin(GL_QUADS);
        glVertex2f(loftX,        LOFT_Y + loftThick);
        glVertex2f(loftX + loftW, LOFT_Y + loftThick);
        glVertex2f(loftX + loftW, LOFT_Y + loftThick + 8);
        glVertex2f(loftX,         LOFT_Y + loftThick + 8);
    glEnd();
    // Loft front face (darker)
    glColor3f(0.40f, 0.23f, 0.07f);
    glBegin(GL_QUADS);
        glVertex2f(loftX,        LOFT_Y);
        glVertex2f(loftX + loftW, LOFT_Y);
        glVertex2f(loftX + loftW, LOFT_Y + loftThick);
        glVertex2f(loftX,         LOFT_Y + loftThick);
    glEnd();
    woodPlanksV(loftX, LOFT_Y, loftW, loftThick, 22.0f, 0.30f, 0.16f, 0.04f);

    // Loft railing spindles
    glColor3f(0.45f, 0.26f, 0.08f);
    int spindles = 14;
    float spindleTop = LOFT_Y + H * 0.09f;
    for (int s = 0; s <= spindles; s++) {
        float sx = loftX + s * loftW / spindles;
        glBegin(GL_QUADS);
            glVertex2f(sx - 3, LOFT_Y + loftThick);
            glVertex2f(sx + 3, LOFT_Y + loftThick);
            glVertex2f(sx + 3, spindleTop);
            glVertex2f(sx - 3, spindleTop);
        glEnd();
    }
    // Railing top bar
    glColor3f(0.52f, 0.30f, 0.10f);
    glBegin(GL_QUADS);
        glVertex2f(loftX,        spindleTop);
        glVertex2f(loftX + loftW, spindleTop);
        glVertex2f(loftX + loftW, spindleTop + 8);
        glVertex2f(loftX,         spindleTop + 8);
    glEnd();

    // ═══════════════════════════════════════════════════════════
    // 8.  LOFT WINDOWS  (two small rectangular windows above loft)
    // ═══════════════════════════════════════════════════════════
    float winY = LOFT_Y + loftThick + H * 0.11f;
    float winH = H * 0.08f;
    float winW = W * 0.12f;
    float win1X = CX - W*0.20f - winW;
    float win2X = CX + W*0.08f;

    for (int w2 = 0; w2 < 2; w2++) {
        float wx = (w2 == 0) ? win1X : win2X;
        // Sky inside window
        filledRect(wx, winY, winW, winH, 0.60f, 0.85f, 0.95f);
        // Window frame
        glColor3f(0.42f, 0.25f, 0.08f);
        glLineWidth(4.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(wx,       winY);
            glVertex2f(wx+winW,  winY);
            glVertex2f(wx+winW,  winY+winH);
            glVertex2f(wx,       winY+winH);
        glEnd();
        // Cross bar
        glBegin(GL_LINES);
            glVertex2f(wx + winW*0.5f, winY);
            glVertex2f(wx + winW*0.5f, winY + winH);
        glEnd();
    }

    // ═══════════════════════════════════════════════════════════
    // 9.  CEILING SUPPORT BEAMS – proper A-frame triangle
    //     Two diagonals meet at the PEAK, horizontal bar connects them
    // ═══════════════════════════════════════════════════════════
    float beamPeakX = CX;
    float beamPeakY = CEIL_PEAK - H*0.04f;
    float beamFeetY = LOFT_Y + loftThick + H*0.04f;
    float beamLeftX  = W * 0.22f;
    float beamRightX = W * 0.78f;

    // Draw as thick quads for solid look
    glColor3f(0.35f, 0.20f, 0.06f);
    float beamThick = 7.0f;

    // Left diagonal beam
    glBegin(GL_QUADS);
        glVertex2f(beamLeftX - beamThick, beamFeetY);
        glVertex2f(beamLeftX + beamThick, beamFeetY);
        glVertex2f(beamPeakX + beamThick, beamPeakY);
        glVertex2f(beamPeakX - beamThick, beamPeakY);
    glEnd();
    // Right diagonal beam
    glBegin(GL_QUADS);
        glVertex2f(beamPeakX - beamThick, beamPeakY);
        glVertex2f(beamPeakX + beamThick, beamPeakY);
        glVertex2f(beamRightX + beamThick, beamFeetY);
        glVertex2f(beamRightX - beamThick, beamFeetY);
    glEnd();

    // Horizontal crossbar connecting the two feet
    float crossY2 = beamFeetY + H*0.005f;
    glBegin(GL_QUADS);
        glVertex2f(beamLeftX - beamThick, crossY2 - beamThick*0.7f);
        glVertex2f(beamRightX + beamThick, crossY2 - beamThick*0.7f);
        glVertex2f(beamRightX + beamThick, crossY2 + beamThick*0.7f);
        glVertex2f(beamLeftX - beamThick, crossY2 + beamThick*0.7f);
    glEnd();

    // Highlight top edges
    glColor3f(0.50f, 0.30f, 0.10f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2f(beamLeftX, beamFeetY); glVertex2f(beamPeakX, beamPeakY);
    glEnd();
    glBegin(GL_LINES);
        glVertex2f(beamPeakX, beamPeakY); glVertex2f(beamRightX, beamFeetY);
    glEnd();

    // ═══════════════════════════════════════════════════════════
    // 10. LEFT SHELF + MILK CANS
    // ═══════════════════════════════════════════════════════════
    float lShelfY = H * 0.42f;
    float lShelfX = 0;
    float lShelfW = W * 0.185f;
    float lShelfH = H * 0.025f;

    // Shelf board
    glColor3f(0.55f, 0.35f, 0.12f);
    glBegin(GL_QUADS);
        glVertex2f(lShelfX,         lShelfY);
        glVertex2f(lShelfX+lShelfW, lShelfY);
        glVertex2f(lShelfX+lShelfW, lShelfY + lShelfH);
        glVertex2f(lShelfX,         lShelfY + lShelfH);
    glEnd();
    glColor3f(0.35f, 0.18f, 0.04f);
    glBegin(GL_QUADS);
        glVertex2f(lShelfX, lShelfY - lShelfH*0.8f);
        glVertex2f(lShelfX+lShelfW, lShelfY - lShelfH*0.8f);
        glVertex2f(lShelfX+lShelfW, lShelfY);
        glVertex2f(lShelfX, lShelfY);
    glEnd();
    outlineRect(lShelfX, lShelfY - lShelfH, lShelfW, lShelfH*2,
                0.25f, 0.12f, 0.03f, 1.5f);

    // Two milk cans on shelf
    float canH = H * 0.18f;
    float canW = W * 0.065f;
    drawMilkCan(W * 0.055f, lShelfY + lShelfH, canW,       canH);
    drawMilkCan(W * 0.125f, lShelfY + lShelfH, canW*0.88f, canH*0.90f);

    // ═══════════════════════════════════════════════════════════
    // 11. TWO BARRELS  (lower left foreground, sitting on floor)
    // ═══════════════════════════════════════════════════════════
    float barH = H * 0.26f;
    float barW = W * 0.115f;
    // Back barrel (slightly right and behind)
    drawBarrel(W * 0.175f, H * 0.005f, barW * 0.90f, barH * 0.88f);
    // Front barrel (bigger, more left)
    drawBarrel(W * 0.095f, H * 0.005f, barW, barH);

    // ═══════════════════════════════════════════════════════════
    // 12. LADDER  (center-left, leaning against loft)
    // ═══════════════════════════════════════════════════════════
    float ladX = W * 0.33f;
    float ladW = W * 0.055f;
    float ladH = H * 0.30f;
    drawLadder(ladX, FLOOR_TOP - H*0.02f, ladW, ladH);

    // ═══════════════════════════════════════════════════════════
    // 13. RIGHT SHELF + CERAMIC POTS + EGG BASKET
    //     Shelf sits lower, extends edge-to-edge on right wall
    // ═══════════════════════════════════════════════════════════
    float rShelfY = H * 0.26f;          // lower than before
    float rShelfX = W * 0.60f;
    float rShelfW = W * 0.40f;          // full to right edge
    float rShelfH = H * 0.028f;

    // Shelf top surface
    glColor3f(0.58f, 0.38f, 0.14f);
    glBegin(GL_QUADS);
        glVertex2f(rShelfX,           rShelfY + rShelfH*0.5f);
        glVertex2f(rShelfX + rShelfW, rShelfY + rShelfH*0.5f);
        glVertex2f(rShelfX + rShelfW, rShelfY + rShelfH);
        glVertex2f(rShelfX,           rShelfY + rShelfH);
    glEnd();
    // Shelf front face (darker)
    glColor3f(0.38f, 0.20f, 0.05f);
    glBegin(GL_QUADS);
        glVertex2f(rShelfX,           rShelfY);
        glVertex2f(rShelfX + rShelfW, rShelfY);
        glVertex2f(rShelfX + rShelfW, rShelfY + rShelfH*0.55f);
        glVertex2f(rShelfX,           rShelfY + rShelfH*0.55f);
    glEnd();
    // Shelf plank lines
    woodPlanksV(rShelfX, rShelfY, rShelfW, rShelfH, 30.0f, 0.28f, 0.14f, 0.03f);
    glColor3f(0.22f, 0.10f, 0.02f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(rShelfX,           rShelfY);
        glVertex2f(rShelfX + rShelfW, rShelfY);
        glVertex2f(rShelfX + rShelfW, rShelfY + rShelfH);
        glVertex2f(rShelfX,           rShelfY + rShelfH);
    glEnd();

    // 4 ceramic pots on right shelf
    float potBase = rShelfY + rShelfH;
    // Pot 1 – small, light tan
    drawPot(W * 0.640f, potBase, W*0.055f, H*0.10f,  0.75f, 0.58f, 0.38f);
    // Pot 2 – large, green-brown
    drawPot(W * 0.705f, potBase, W*0.072f, H*0.120f, 0.38f, 0.45f, 0.25f);
    // Pot 3 – medium, plain brown
    drawPot(W * 0.785f, potBase, W*0.058f, H*0.100f, 0.58f, 0.38f, 0.18f);
    // Pot 4 – small
    drawPot(W * 0.852f, potBase, W*0.048f, H*0.085f, 0.55f, 0.35f, 0.16f);

    // Egg basket at far right
    drawEggBasket(W * 0.925f, potBase, W*0.088f, H*0.105f);

    // ═══════════════════════════════════════════════════════════
    // 14. THREE PENDANT LIGHTS
    // ═══════════════════════════════════════════════════════════
    float lightTop = beamY;   // hang from main beam
    float coneH    = H * 0.38f;

    drawPendantLight(W * 0.18f, lightTop, coneH * 0.75f);  // left
    drawPendantLight(CX,        lightTop, coneH);           // center (biggest cone)
    drawPendantLight(W * 0.82f, lightTop, coneH * 0.75f);  // right

    // ═══════════════════════════════════════════════════════════
    // 15. AMBIENT WARM OVERLAY  (subtle warmth over whole scene)
    // ═══════════════════════════════════════════════════════════
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.85f, 0.55f, 0.10f, 0.07f);
    glBegin(GL_QUADS);
        glVertex2f(0,       0);
        glVertex2f((float)W, 0);
        glVertex2f((float)W, (float)H);
        glVertex2f(0,       (float)H);
    glEnd();
    glDisable(GL_BLEND);
}
