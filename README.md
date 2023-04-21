Programmation parallèle et multi-thread safety pour la manipulation d'objets 3D
===============================================================================

L'obectif de notre travail a été de rendre thread-safe l'utlisation en multithreading des cartes combinatoires de CGAL. Plus précisément, rendre thread-safe les structures 'Linear Cell Complex', 'Combinatorial Map' et 'Generalized Map'. Ce sont donc sur ces packages que nous nous sommes concentrés.
Le gros du travail a été de fournir une implémentation thread-safe d'une structure de données Bitset, celle de la STL n'étant pas safe. Nous avons pu trouver [une implémentation](https://github.com/facebook/folly/blob/main/folly/ConcurrentBitSet.h) venant de Folly (Facebook Open Source Library).

Une autre partie de notre travail a été de fournir à l'utilisateur la possibilité de choisir, à la compilation, d'utiliser la version thread-safe des cartes combinatoires ou non. Cela s'est fait au moyen de templates et de 'tags' via la library Boost. On peut alors, à la compilation, choisir d'utiliser l'AtomicBitset (thread-safe) ou le std::bitset de l'implémentation originale (l'atomic bitset n'est pas la seule différence entre l'implémentation thread-safe des cartes combinatoires et la version non-safe).

Nous avons également eu l'occasion de comparer différentes approches permettant de synchroniser différents threads travaillant sur la même carte combinatoire. Notamment, nous avons comparé l'impact sur les performances et sur l'utilisation de mémoire de l'AtomicBitset de Folly avec une implémentation maison basée sur celle de Folly d'un 'CharBitset', complètement non bloquant mais utilisant bien plus de mémoire.

Le travail effectué et la documentation pour le travail de développement effectué (s’il y a eu du travail de développement)




![CGAL](Installation/doc_html/images/cgal_2013_grey.png)

The Computational Geometry Algorithms Library (CGAL) is a C++ library that
aims to provide easy access to efficient and reliable algorithms in
computational geometry.

CGAL Releases
=============
The primary vector of distribution of CGAL are source tarballs, released
twice a year, announced on [the web site of CGAL](https://www.cgal.org/).

Getting Started with CGAL
=========================

**Since version 5.0, CGAL is a header-only library, meaning that
it is no longer needed to build CGAL libraries before it can be used.**

Head over to the [CGAL manual](https://doc.cgal.org/latest/Manual/general_intro.html)
for usage guides and tutorials that will get you started smoothly.

License
=======
See the file [LICENSE.md](LICENSE.md).

CGAL Git Repository Layout
==========================

The Git repository of CGAL has a different layout from release tarballs. It
contains a `CMakeLists.txt` file that serves as anchor for configuring and building programs,
and a set of subfolders, so called *packages*. Most packages
implement a data structure or an algorithm for CGAL (e.g., `Convex_hull_2`,
or `Triangulation_3`); however some packages serve special needs:

* `Installation` - meta-files and CMake-support
* `Maintenance` - infrastructural support
* `Core`, `CGALimageIO`, `Qt_widget`, `GraphicsView` - component libraries
* `Scripts` - scripts to simplify developer's and user's work
* `Testsuite` - infrastructure for testsuite
* `Documentation` - infrastructure for CGAL's manual
* `STL_Extension` - extensions to the standard template library

More Information
================
* [The CGAL web site](https://www.cgal.org/)
* [Latest CGAL release documentation pages](https://doc.cgal.org/)
* [Latest CGAL master documentation pages, updated once a week](https://cgal.geometryfactory.com/CGAL/doc/master/)
* [CGAL daily testsuite results](https://cgal.geometryfactory.com/CGAL/testsuite/)
* [Guidelines for CGAL developers](https://github.com/CGAL/cgal/wiki/Guidelines) and [Information for new developers](https://github.com/CGAL/cgal/wiki/Information-for-New-Developers)
