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

    // Rotação em torno de um eixo arbitrário (axis) por um ângulo (angle em graus)
    static Mat4 rotate(double angle, Vec3 axis) {
        Mat4 res;
        double rad = angle * M_PI / 180.0;
        double c = cos(rad);
        double s = sin(rad);
        double t = 1.0 - c;
        
        Vec3 a = axis.normalize(); // O eixo DEVE ser normalizado

        res.m[0][0] = c + a.x*a.x*t;      res.m[0][1] = a.x*a.y*t - a.z*s;  res.m[0][2] = a.x*a.z*t + a.y*s;
        res.m[1][0] = a.y*a.x*t + a.z*s;  res.m[1][1] = c + a.y*a.y*t;      res.m[1][2] = a.y*a.z*t - a.x*s;
        res.m[2][0] = a.z*a.x*t - a.y*s;  res.m[2][1] = a.z*a.y*t + a.x*s;  res.m[2][2] = c + a.z*a.z*t;
        
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

    // =============================================================
    // SISTEMA DE COORDENADAS DE CÂMERA (LookAt)
    // =============================================================
    // Cria a Matriz "World to Camera" (View Matrix)
    // eye: Onde a câmera está
    // center: Para onde ela está olhando
    // up: Qual direção é "pra cima" (geralmente 0,1,0)
    static Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up) {
        // 1. Calcular o vetor W (Z da câmera) - Aponta para TRÁS da visão
        // (O sistema é mão direita: Z sai da tela em direção ao olho)
        Vec3 f = (center - eye).normalize(); // Vetor Frente
        Vec3 w = -f;                         // Vetor Trás (Z camera)

        // 2. Calcular o vetor U (X da câmera) - Aponta para a DIREITA
        // Produto vetorial entre UP do mundo e o Z da câmera
        Vec3 u = up.cross(w).normalize();

        // 3. Calcular o vetor V (Y da câmera) - Aponta para CIMA (Ortogonal real)
        Vec3 v = w.cross(u);

        // 4. Montar a Matriz View (Rotação * Translação)
        // [ Ux  Uy  Uz  -dot(u, eye) ]
        // [ Vx  Vy  Vz  -dot(v, eye) ]
        // [ Wx  Wy  Wz  -dot(w, eye) ]
        // [  0   0   0       1       ]
        Mat4 res;
        res.m[0][0] = u.x; res.m[0][1] = u.y; res.m[0][2] = u.z; res.m[0][3] = -u.dot(eye);
        res.m[1][0] = v.x; res.m[1][1] = v.y; res.m[1][2] = v.z; res.m[1][3] = -v.dot(eye);
        res.m[2][0] = w.x; res.m[2][1] = w.y; res.m[2][2] = w.z; res.m[2][3] = -w.dot(eye);
        res.m[3][0] = 0.0; res.m[3][1] = 0.0; res.m[3][2] = 0.0; res.m[3][3] = 1.0;
        
        return res;
    }
};

#endif