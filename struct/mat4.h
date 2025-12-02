#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include <iostream>
#include "vec3.h"

struct Mat4 {
    double m[4][4]; // Matriz 4x4

    // Construtor: Cria uma matriz Identidade por padrão
    Mat4() {
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                m[i][j] = (i == j) ? 1.0 : 0.0;
    }

    // =============================================================
    // AULA 15: Composição de Transformações (Multiplicação)
    // =============================================================
    // Permite fazer: MatrizFinal = Translação * Rotação * Escala
    Mat4 operator*(const Mat4& n) const {
        Mat4 res;
        for(int i=0; i<4; i++) {
            for(int j=0; j<4; j++) {
                res.m[i][j] = 0;
                for(int k=0; k<4; k++) {
                    res.m[i][j] += m[i][k] * n.m[k][j];
                }
            }
        }
        return res;
    }

    // Transforma um PONTO (w = 1). Usado para 'origem' do raio e vértices.
    // Translação AFETA pontos.
    Vec3 point(const Vec3& v) const {
        double x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3];
        double y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3];
        double z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3];
        double w = m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3];
        
        // Normalização Homogênea (Aula 13)
        if (w != 1.0 && w != 0.0) {
            return {x/w, y/w, z/w};
        }
        return {x, y, z};
    }

    // Transforma um VETOR (w = 0). Usado para 'direção' do raio.
    // Translação NÃO afeta vetores (só rotação e escala).
    Vec3 vector(const Vec3& v) const {
        double x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
        double y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
        double z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
        return {x, y, z};
    }

    // =============================================================
    // AULA 13: Translação e Rotação nos eixos canônicos
    // =============================================================
    
    static Mat4 identity() {
        return Mat4();
    }

    static Mat4 translate(double x, double y, double z) {
        Mat4 res;
        res.m[0][3] = x;
        res.m[1][3] = y;
        res.m[2][3] = z;
        return res;
    }

    static Mat4 scale(double x, double y, double z) {
        Mat4 res;
        res.m[0][0] = x;
        res.m[1][1] = y;
        res.m[2][2] = z;
        return res;
    }

    // Ângulo em GRAUS
    static Mat4 rotateX(double angle) {
        Mat4 res;
        double rad = angle * M_PI / 180.0;
        double c = cos(rad);
        double s = sin(rad);
        res.m[1][1] = c; res.m[1][2] = -s;
        res.m[2][1] = s; res.m[2][2] = c;
        return res;
    }

    static Mat4 rotateY(double angle) {
        Mat4 res;
        double rad = angle * M_PI / 180.0;
        double c = cos(rad);
        double s = sin(rad);
        res.m[0][0] = c;  res.m[0][2] = s;
        res.m[2][0] = -s; res.m[2][2] = c;
        return res;
    }

    static Mat4 rotateZ(double angle) {
        Mat4 res;
        double rad = angle * M_PI / 180.0;
        double c = cos(rad);
        double s = sin(rad);
        res.m[0][0] = c; res.m[0][1] = -s;
        res.m[1][0] = s; res.m[1][1] = c;
        return res;
    }

    // =============================================================
    // AULA 14: Cisalhamento (Shear)
    // =============================================================
    // xy: Desloca X baseado em Y
    // xz: Desloca X baseado em Z... etc.
    static Mat4 shear(double xy, double xz, double yx, double yz, double zx, double zy) {
        Mat4 res;
        res.m[0][1] = xy; res.m[0][2] = xz;
        res.m[1][0] = yx; res.m[1][2] = yz;
        res.m[2][0] = zx; res.m[2][1] = zy;
        return res;
    }

    // Transposta (Necessária para corrigir Normais se usar Shear/Scale)
    Mat4 transpose() const {
        Mat4 res;
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                res.m[i][j] = m[j][i];
        return res;
    }

    // Inversa (Algoritmo genérico via Cofatores) - Necessária para o Ray Tracing
    Mat4 inverse() const {
        // Implementação simplificada de inversão 4x4
        // (Para brevidade, estou usando um método padrão. Se precisar da lógica matemática pura, avise)
        double inv[16], det;
        double me[16];
        int k = 0;
        for(int i=0; i<4; i++) for(int j=0; j<4; j++) me[k++] = m[i][j];

        inv[0] = me[5]  * me[10] * me[15] - me[5]  * me[11] * me[14] - me[9]  * me[6]  * me[15] + me[9]  * me[7]  * me[14] + me[13] * me[6]  * me[11] - me[13] * me[7]  * me[10];
        inv[4] = -me[4]  * me[10] * me[15] + me[4]  * me[11] * me[14] + me[8]  * me[6]  * me[15] - me[8]  * me[7]  * me[14] - me[12] * me[6]  * me[11] + me[12] * me[7]  * me[10];
        inv[8] = me[4]  * me[9] * me[15] - me[4]  * me[11] * me[13] - me[8]  * me[5] * me[15] + me[8]  * me[7] * me[13] + me[12] * me[5] * me[11] - me[12] * me[7] * me[9];
        inv[12] = -me[4]  * me[9] * me[14] + me[4]  * me[10] * me[13] + me[8]  * me[5] * me[14] - me[8]  * me[6] * me[13] - me[12] * me[5] * me[10] + me[12] * me[6] * me[9];
        inv[1] = -me[1]  * me[10] * me[15] + me[1]  * me[11] * me[14] + me[9]  * me[2] * me[15] - me[9]  * me[3] * me[14] - me[13] * me[2] * me[11] + me[13] * me[3] * me[10];
        inv[5] = me[0]  * me[10] * me[15] - me[0]  * me[11] * me[14] - me[8]  * me[2] * me[15] + me[8]  * me[3] * me[14] + me[12] * me[2] * me[11] - me[12] * me[3] * me[10];
        inv[9] = -me[0]  * me[9] * me[15] + me[0]  * me[11] * me[13] + me[8]  * me[1] * me[15] - me[8]  * me[3] * me[13] - me[12] * me[1] * me[11] + me[12] * me[3] * me[9];
        inv[13] = me[0]  * me[9] * me[14] - me[0]  * me[10] * me[13] - me[8]  * me[1] * me[14] + me[8]  * me[2] * me[13] + me[12] * me[1] * me[10] - me[12] * me[2] * me[9];
        inv[2] = me[1]  * me[6] * me[15] - me[1]  * me[7] * me[14] - me[5]  * me[2] * me[15] + me[5]  * me[3] * me[14] + me[13] * me[2] * me[7] - me[13] * me[3] * me[6];
        inv[6] = -me[0]  * me[6] * me[15] + me[0]  * me[7] * me[14] + me[4]  * me[2] * me[15] - me[4]  * me[3] * me[14] - me[12] * me[2] * me[7] + me[12] * me[3] * me[6];
        inv[10] = me[0]  * me[5] * me[15] - me[0]  * me[7] * me[13] - me[4]  * me[1] * me[15] + me[4]  * me[3] * me[13] + me[12] * me[1] * me[7] - me[12] * me[3] * me[5];
        inv[14] = -me[0]  * me[5] * me[14] + me[0]  * me[6] * me[13] + me[4]  * me[1] * me[14] - me[4]  * me[2] * me[13] - me[12] * me[1] * me[6] + me[12] * me[2] * me[5];
        inv[3] = -me[1] * me[6] * me[11] + me[1] * me[7] * me[10] + me[5] * me[2] * me[11] - me[5] * me[3] * me[10] - me[9] * me[2] * me[7] + me[9] * me[3] * me[6];
        inv[7] = me[0] * me[6] * me[11] - me[0] * me[7] * me[10] - me[4] * me[2] * me[11] + me[4] * me[3] * me[10] + me[8] * me[2] * me[7] - me[8] * me[3] * me[6];
        inv[11] = -me[0] * me[5] * me[11] + me[0] * me[7] * me[9] + me[4] * me[1] * me[11] - me[4] * me[3] * me[9] - me[8] * me[1] * me[7] + me[8] * me[3] * me[5];
        inv[15] = me[0] * me[5] * me[10] - me[0] * me[6] * me[9] - me[4] * me[1] * me[10] + me[4] * me[2] * me[9] + me[8] * me[1] * me[6] - me[8] * me[2] * me[5];

        det = me[0] * inv[0] + me[1] * inv[4] + me[2] * inv[8] + me[3] * inv[12];

        if (det == 0) return Mat4(); // Erro: sem inversa, retorna identidade

        det = 1.0 / det;
        Mat4 res;
        k = 0;
        for(int i=0; i<4; i++) for(int j=0; j<4; j++) res.m[i][j] = inv[k++] * det;
        return res;
    }
};

#endif