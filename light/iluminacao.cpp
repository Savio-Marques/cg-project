#include "iluminacao.h"
#include <cmath>
#include <algorithm>
#include <limits>

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
        
        Vec3 vetorLuz;
        double distanciaLuz;

        // --- 1. CONFIGURAR O VETOR DA LUZ ---
        if (luz.type == DIRECTIONAL) {
            // Luz direcional não tem origem, ela "vem de" uma direção.
            // O vetorLuz aponta PARA a luz, então invertemos a direção dela.
            vetorLuz = (-luz.direction).normalize();
            distanciaLuz = std::numeric_limits<double>::infinity(); // Distância infinita
        } 
        else { 
            // POINT ou SPOT
            // Vetor que vai do objeto até a luz
            Vec3 dir = luz.position - rec.ponto;
            distanciaLuz = dir.norm();
            vetorLuz = dir.normalize();
        }

        // --- 2. VERIFICAÇÃO DE SPOT (Lanterna) ---
        // Se for SPOT, verificamos se o ponto está dentro do cone
        if (luz.type == SPOT) {
            // Calcula o ângulo entre o raio da luz e a direção do spot
            double theta = (-vetorLuz).dot(luz.direction.normalize());
            
            // CONVERSÃO AUTOMÁTICA: Graus -> Cosseno
            // O usuario passou "20" no main, aqui convertemos para o formato matemático
            // M_PI precisa do include <cmath>
            double cutoffCosseno = std::cos(luz.cutoff * M_PI / 180.0);

            // Se o ângulo atual for MENOR que o limite (valor do cosseno menor), corta
            // Lembre-se: cos(0) = 1, cos(90) = 0. Quanto maior o ângulo, menor o cosseno.
            if (theta < cutoffCosseno) {
                continue; // Fora do cone de luz
            }
        }

        // --- 3. CÁLCULO DE SOMBRA ---
        Ray raioSombra = {rec.ponto + rec.normal * 1e-4, vetorLuz};
        bool emSombra = false;
        HitRecord rec_sombra;

        for (const auto& obj : cena) {
            if (obj->intersecta(raioSombra, 0.001, distanciaLuz, rec_sombra)) {
                emSombra = true;
                break;
            }
        }

        if (!emSombra) {
            // O RESTO DA FÓRMULA DE PHONG É IGUAL PARA TODOS!
            
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