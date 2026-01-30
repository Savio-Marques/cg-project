#include "iluminacao.h"
#include <cmath>
#include <algorithm>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Vec3 calcularIluminacao(
    const Ray& raioVisao, 
    const HitRecord& rec, 
    const std::vector<Objeto*>& cena, 
    const std::vector<Light>& luzes, 
    const Vec3& luzAmbiente,
    const Vec3& Ka, const Vec3& Kd, const Vec3& Ke
) {
    Vec3 corFinal = {0, 0, 0};

    // Ambiente (Luz base)
    // Ia = Ka * LuzAmbiente
    Vec3 I_amb = luzAmbiente.hadamard(Ka);
    corFinal = corFinal + I_amb;

    // Para cada luz
    for (const auto& luz : luzes) {
        
        Vec3 vetorLuz;
        double distanciaLuz;

        // Configura vetor da luz
        if (luz.type == DIRECTIONAL) {
            // Luz direcional não tem origem
            // O vetorLuz aponta para a luz, inverte a direção 
            vetorLuz = (-luz.direction).normalize();
            distanciaLuz = std::numeric_limits<double>::infinity(); // Distância infinita
        } 
        else { 
            // Point ou Spot
            // Vetor que vai do objeto até a luz
            Vec3 dir = luz.position - rec.ponto;
            distanciaLuz = dir.norm();
            vetorLuz = dir.normalize();
        }

        // Verificação Spot
        // Se for Spot, verifica se o ponto está dentro do cone
        if (luz.type == SPOT) {
            // Calcula o ângulo entre o raio da luz e a direção do spot
            double theta = (-vetorLuz).dot(luz.direction.normalize());
            
            // Conversão automática de graus pra cosseno
            double cutoffCosseno = std::cos(luz.cutoff * M_PI / 180.0);

            // Se o ângulo atual for menor que o limite é cortado
            // Quanto maior o ângulo, menor o cosseno
            if (theta < cutoffCosseno) {
                continue; // Fora do cone de luz
            }
        }

        // Sombra
        Ray raioSombra = {rec.ponto + rec.normal * 1e-4, vetorLuz};
        bool emSombra = false;
        HitRecord rec_sombra;

        // Verifica se existe algum objeto entre o ponto atual e a fonte de luz.
        for (const auto& obj : cena) {
            if (obj->intersecta(raioSombra, 0.001, distanciaLuz, rec_sombra)) {
                emSombra = true;
                break;
            }
        }

        // A luz só é calculada se o ponto não estiver em sombra.
        if (!emSombra) {
            
            // Difusa
            double dot_l_n = std::max(0.0, vetorLuz.dot(rec.normal));
            Vec3 I_dif = luz.intensity.hadamard(Kd) * dot_l_n;
            corFinal = corFinal + I_dif;

            // Especular
            if (dot_l_n > 0) { // Só tem brilho se tiver luz difusa
                Vec3 v = (raioVisao.origem - rec.ponto).normalize();
                Vec3 r_ref = (rec.normal * (2.0 * vetorLuz.dot(rec.normal)) - vetorLuz).normalize();
                double spec = std::pow(std::max(0.0, v.dot(r_ref)), rec.mat.shininess);
                Vec3 I_spec = luz.intensity.hadamard(Ke) * spec;
                corFinal = corFinal + I_spec;
            }
        }
    }
    
    return corFinal;

    // Garante que não ultrapasse 1.0 (branco)
    return corFinal.clamp(0.0, 1.0);
}