#include <vector>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct vec3d {
    float x, y, z;
};

struct triangle {
    vec3d p[3];
};

struct mesh {
    std::vector<triangle> tris; // defining the object
};

struct mat4x4 {
    float m[4][4] = { 0 };
};


class olcEngine3D : public olc::PixelGameEngine {
    public:
        olcEngine3D() {
            sAppName = "3D Demo";

        }

    private:
        mesh meshCube;
        mat4x4 matProj;

        float fTheta;

        void MultiplyMatrixVector(vec3d &in, vec3d &out, mat4x4 &m) {
            out.x = in.x * m.m[0][0] + in.y * m.m[1][0] + in.z * m.m[2][0] + m.m[3][0];
            out.y = in.x * m.m[0][1] + in.y * m.m[1][1] + in.z * m.m[2][1] + m.m[3][1];
            out.z = in.x * m.m[0][2] + in.y * m.m[1][2] + in.z * m.m[2][2] + m.m[3][2];
            float w = in.x * m.m[0][3] + in.y * m.m[1][3] + in.z * m.m[2][3] + m.m[3][3];

            if (w != 0.0f)
            {
                out.x /= w; out.y /= w; out.z /= w;
            }
        }

    public:
        bool OnUserCreate() override {
            meshCube.tris = {
                // SOUTH
                { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

                // EAST                                                      
                { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

                // NORTH                                                     
                { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

                // WEST                                                      
                { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

                // TOP                                                       
                { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

                // BOTTOM                                                    
                { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
            };

            // Projection Matrix
            float fNear = 0.1f;
            float fFar = 1000.0;
            float fFov = 90.0f;
            float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
            float fFovRad = 1.0f / tan(fFov * 0.5f / 180.0f * 3.14159f);

            matProj.m[0][0] = fAspectRatio * fFovRad;
            matProj.m[1][1] = fFovRad;
            matProj.m[2][2] = fFar / (fFar - fNear);
            matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
            matProj.m[2][3] = 1.0f;
            matProj.m[3][3] = 0.0f;

            return true;
        }

        bool OnUserUpdate(float fElapsedTime) override {
            // Clear screen
            FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

            mat4x4 matRotZ, matRotX;
            fTheta += 1.0f * fElapsedTime;

            // Rotation Z
            matRotZ.m[0][0] = cosf(fTheta);
            matRotZ.m[0][1] = sinf(fTheta);
            matRotZ.m[1][0] = -sinf(fTheta);
            matRotZ.m[1][1] = cosf(fTheta);
            matRotZ.m[2][2] = 1;
            matRotZ.m[3][3] = 1;

            // Rotation X
            matRotX.m[0][0] = 1;
            matRotX.m[1][1] = cosf(fTheta * 0.5f);
            matRotX.m[1][2] = sinf(fTheta * 0.5f);
            matRotX.m[2][1] = -sinf(fTheta * 0.5f);
            matRotX.m[2][2] = cosf(fTheta * 0.5f);
            matRotX.m[3][3] = 1;

            // Draw Triangles
            for (auto tri : meshCube.tris) {
                triangle triProjected, triTranslated, triRotatedZ, triRotatedX;

                // Rotate around Z-axis
                MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
                MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
                MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

                // Rotate around X-axis
                MultiplyMatrixVector(triRotatedZ.p[0], triRotatedX.p[0], matRotX);
                MultiplyMatrixVector(triRotatedZ.p[1], triRotatedX.p[1], matRotX);
                MultiplyMatrixVector(triRotatedZ.p[2], triRotatedX.p[2], matRotX);
                
                // Offset into the screen
                triTranslated = triRotatedX;
                triTranslated.p[0].z = triRotatedX.p[0].z + 3.0f;
                triTranslated.p[1].z = triRotatedX.p[1].z + 3.0f;
                triTranslated.p[2].z = triRotatedX.p[2].z + 3.0f;
                
                vec3d normal, line1, line2;
                line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
                line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
                line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

                line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
                line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
                line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

                normal.x = line1.y * line2.z - line1.z * line2.y;
                normal.y = line1.z * line2.x - line1.x * line2.z;
                normal.z = line1.x * line2.y - line1.y * line2.x;

                float lenNormal = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
                normal.x /= lenNormal; normal.y /= lenNormal; normal.z /= lenNormal;

                if (normal.z < 0) { // triangle visible
                    // Project triangles from 3D to 2D
                    MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
                    MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
                    MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

                    // Scale into view
                    triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
                    triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
                    triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

                    triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
                    triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
                    triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
                    triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
                    triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
                    triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

                    DrawTriangle(triProjected.p[0].x, triProjected.p[0].y, triProjected.p[1].x, triProjected.p[1].y, triProjected.p[2].x, triProjected.p[2].y, olc::WHITE);
                }
            }

            return true;
        }
};

int main() {
    olcEngine3D demo;
    if (demo.Construct(256, 240, 4, 4, false, true))
        demo.Start();

    return 0;
}