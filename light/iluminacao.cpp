#include "iluminacao.h"
#include <cmath>
#include <algorithm>

Vec3 calcularIluminacao(
    const Ray& raioVisao, 
    const HitRecord& rec, 
    const std::vector<Objeto*>& cena, 
    const std::vector<Light>& luzes, 
    const Vec3& luzAmbiente,
    const Vec3& Ka, const Vec3& Kd, const Vec3& Ke
) {
    Vec3 corFinal = {0, 0, 0};

    // 1. Componente Ambiente (Luz base)
    // Fórmula: Ia = Ka * LuzAmbiente
    Vec3 I_amb = luzAmbiente.hadamard(Ka);
    corFinal = corFinal + I_amb;

    // Para cada luz na cena (suporta múltiplas luzes agora!)
    for (const auto& luz : luzes) {
        
        // Vetor que vai do ponto atingido até a luz
        Vec3 vetorLuz = (luz.position - rec.ponto).normalize();
        double distanciaLuz = (luz.position - rec.ponto).norm();

        // --- CÁLCULO DE SOMBRA ---
        // Lança um raio do ponto em direção à luz
        Ray raioSombra = {rec.ponto + rec.normal * 1e-4, vetorLuz}; // 1e-4 evita acne
        bool emSombra = false;

        HitRecord rec_sombra;
        for (const auto& obj : cena) {
            // Se bater em algo antes de chegar na luz...
            if (obj->intersecta(raioSombra, 0.001, distanciaLuz, rec_sombra)) {
                emSombra = true;
                break;
            }
        }

        if (!emSombra) {
            // 2. Componente Difusa (Cor Fosca)
            // O quanto a normal está alinhada com a luz?
            double dot_l_n = std::max(0.0, vetorLuz.dot(rec.normal));
            Vec3 I_dif = luz.intensity.hadamard(Kd) * dot_l_n;
            corFinal = corFinal + I_dif;

            // 3. Componente Especular (Brilho)
            // Reflexo da luz na superfície
            Vec3 v = (raioVisao.origem - rec.ponto).normalize(); // Vetor para o olho
            Vec3 r_ref = (rec.normal * (2.0 * vetorLuz.dot(rec.normal)) - vetorLuz).normalize();
            
            double dot_v_r = std::max(0.0, v.dot(r_ref));
            double dot_v_r_m = std::pow(dot_v_r, rec.mat.shininess);
            
            Vec3 I_spec = luz.intensity.hadamard(Ke) * dot_v_r_m;
            corFinal = corFinal + I_spec;
        }
    }

    // Garante que não ultrapasse 1.0 (branco)
    return corFinal.clamp(0.0, 1.0);
}