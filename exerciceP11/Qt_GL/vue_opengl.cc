#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "Masse.h"
#include "Tissu.h"
#include "Systeme.h"
#include "Objet.h"
using namespace std;
// ======================================================================

void VueOpenGL::dessine(Masse const& masse) {
  QMatrix4x4 matrice;
  matrice.translate(masse.position().x(), masse.position().y(), masse.position().z());
  matrice.scale(0.25);
  dessineCube(matrice);
};

void VueOpenGL::dessine(Tissu const& tissu) {
    cout << "on commence la boucle sur les masses du tissu" << endl;
    for (auto masse : tissu.vector_masse()) {
        cout << "on essaie de dessiner la masse "<< endl;
        dessine(*masse);
        cout << "masse dessiné " << endl;
    }
};

void VueOpenGL::dessine(Systeme const& systeme) {
    cout << "on commence la boucle sur le vector_objet du systeme"<< endl;
    for (auto tissu : systeme.vector_objet()) { cout << tissu << endl; }
    for (auto tissu : systeme.vector_objet()) {
        cout << "on essaie de dessiner un objet" << endl;
        tissu->dessine_sur(*this);
        cout << "un objet a été dessiné "<< endl;
    }
};

void VueOpenGL::dessine(Objet const& objet) {
    QMatrix4x4 matrice;
    dessineCube(matrice);
}
// ======================================================================
void VueOpenGL::init()
{
  /* Initialise notre vue OpenGL.
   * Dans cet exemple, nous créons et activons notre shader.
   *
   * En raison du contenu des fichiers *.glsl, le shader de cet exemple
   * NE permet QUE de dessiner des primitives colorées
   * (pas de textures, brouillard, reflets de la lumière ou autres).
   *
   * Il est séparé en deux parties VERTEX et FRAGMENT.
   * Le VERTEX :
   * - récupère pour chaque sommet des primitives de couleur (dans
   *     l'attribut couleur) et de position (dans l'attribut sommet)
   * - multiplie l'attribut sommet par les matrices 'vue_modele' et
   *     'projection' et donne le résultat à OpenGL
   *   - passe la couleur au shader FRAGMENT.
   *
   * Le FRAGMENT :
   *   - applique la couleur qu'on lui donne
   */

  prog.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/vertex_shader.glsl");
  prog.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");

  /* Identifie les deux attributs du shader de cet exemple
   * (voir vertex_shader.glsl).
   *
   * L'attribut identifié par 0 est particulier, il permet d'envoyer un
   * nouveau "point" à OpenGL
   *
   * C'est pourquoi il devra obligatoirement être spécifié et en dernier
   * (après la couleur dans cet exemple, voir plus bas).
   */

  prog.bindAttributeLocation("sommet",  SommetId);
  prog.bindAttributeLocation("couleur", CouleurId);

  // Activation du shader
  prog.bind();

  /* Activation du "Test de profondeur" et du "Back-face culling"
   * Le Test de profondeur permet de dessiner un objet à l'arrière-plan
   * partielement caché par d'autres objets.
   *
   * Le Back-face culling consiste à ne dessiner que les face avec ordre
   * de déclaration dans le sens trigonométrique.
   */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  initializePosition();
}

// ======================================================================
void VueOpenGL::initializePosition()
{
  // position initiale
  matrice_vue.setToIdentity();
  //matrice_vue.rotate(30.0, 0.0, 1.0, 0.0);
  matrice_vue.rotate(-90.0, 1.0, 0.0, 0.0);
  matrice_vue.translate(0.0, 5.0, 2.0);

}

// ======================================================================
void VueOpenGL::translate(double x, double y, double z)
{
  /* Multiplie la matrice de vue par LA GAUCHE.
   * Cela fait en sorte que la dernière modification apportée
   * à la matrice soit appliquée en dernier (composition de fonctions).
   */
  QMatrix4x4 translation_supplementaire;
  translation_supplementaire.translate(x, y, z);
  matrice_vue = translation_supplementaire * matrice_vue;
}

// ======================================================================
void VueOpenGL::rotate(double angle, double dir_x, double dir_y, double dir_z)
{
  // Multiplie la matrice de vue par LA GAUCHE
  QMatrix4x4 rotation_supplementaire;
  rotation_supplementaire.rotate(angle, dir_x, dir_y, dir_z);
  matrice_vue = rotation_supplementaire * matrice_vue;
}

// ======================================================================
void VueOpenGL::dessineCube (QMatrix4x4 const& point_de_vue)
{
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  glBegin(GL_QUADS);
  // face coté X = +1
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);

  // face coté X = -1
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0); // vert
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);

  // face coté Y = +1
  prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0); // bleu
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);

  // face coté Y = -1
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 1.0); // cyan
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);

  // face coté Z = +1
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 0.0); // jaune
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);

  // face coté Z = -1
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 1.0); // magenta
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);

  glEnd();
}
