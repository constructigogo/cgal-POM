// Copyright (c) 2010-2011 CNRS and LIRIS' Establishments (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL$
// $Id$
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Guillaume Damiand <guillaume.damiand@liris.cnrs.fr>
//
#ifndef CGAL_DART_H
#define CGAL_DART_H 1

#include <CGAL/assertions.h>
#include <CGAL/tags.h>
#include <CGAL/tuple.h>
#include <CGAL/Cell_attribute.h>

namespace CGAL {

  template <class, class, class, class>
  class Compact_container;

  template <class, class>
  class Concurrent_compact_container;

  template <class, class, class, class>
  class Compact_container_with_index;

  template<unsigned int, class, class>
  class Combinatorial_map_storage_1;

  template<unsigned int, class, class>
  class Combinatorial_map_storage_with_index;

  template<unsigned int, class, class>
  class Generalized_map_storage_1;

  template<unsigned int, class, class>
  class Generalized_map_storage_with_index;

  template<unsigned int, unsigned int, class, class, class>
  class CMap_linear_cell_complex_storage_1;

  template<unsigned int, unsigned int, class, class, class>
  class CMap_linear_cell_complex_storage_with_index;

  template<unsigned int, unsigned int, class, class, class>
  class GMap_linear_cell_complex_storage_1;

  template<unsigned int, unsigned int, class, class, class>
  class GMap_linear_cell_complex_storage_with_index;

  namespace internal {

    template<class, class>
    struct Init_id;

  } // end namespace internal

  /** @file Dart.h
   * Definition of nD dart.
   */

  /**
   * Specialization of methods that are different in the concurrent
   * and non-concurrent case
   */
  template<typename Concurrent_tag, class Dart>
  struct Dart_operations_
  {
      typedef typename Dart::Bitset_type Bitset_type;

  public:
      void copy_marks(const Bitset_type& from, Bitset_type& to) const
      { to = from; }
  };

  template<class Dart>
  struct Dart_operations_<CGAL::Tag_true, Dart>
  {
      typedef typename Dart::Bitset_type Bitset_type;

  public:
      void copy_marks(const Bitset_type& from, Bitset_type& to) const
      {
          for (unsigned int i = 0; i < Dart::NB_MARKS; i++)
          {
              if(from[i]) to.set(i);
              else to.reset(i);
          }
      }
  };

  /** Definition of nD dart without information.
   * The_dart class describes an nD dart (basic element of a combinatorial or generalized map).
   * A dart is composed with descriptor towards its neighbors,
   * a bitset containing Boolean marks, and descriptor towards enabled attributes.
   * n is the dimension of the space (2 for 2D, 3 for 3D...)
   * Refs the ref class
   */
  template <unsigned int d, typename Refs, class WithId>
  struct Dart_without_info: public Add_id<WithId>
  {
  public:
    template <class, class>
    friend class Dart_operations_;

    template <class, class, class, class>
    friend class Compact_container;

    template <class, class>
    friend class Concurrent_compact_container;

    template <class, class, class, class>
    friend class Compact_container_with_index;

    template<unsigned int, class, class>
    friend class Combinatorial_map_storage_1;

    template<unsigned int, class, class>
    friend class Combinatorial_map_storage_with_index;

    template<unsigned int, class, class>
    friend class Generalized_map_storage_1;

    template<unsigned int, class, class>
    friend class Generalized_map_storage_with_index;

    template<unsigned int, unsigned int, class, class, class>
    friend class CMap_linear_cell_complex_storage_1;

    template<unsigned int, unsigned int, class, class, class>
    friend class CMap_linear_cell_complex_storage_with_index;

    template<unsigned int, unsigned int, class, class, class>
    friend class GMap_linear_cell_complex_storage_1;

    template<unsigned int, unsigned int, class, class, class>
    friend class GMap_linear_cell_complex_storage_with_index;

    template<class, class>
    friend struct internal::Init_id;

    typedef Dart_without_info<d,Refs, WithId>                            Self;
    typedef typename Refs::Bitset_type                                   Bitset_type;
    typedef typename Refs::Dart_descriptor                               Dart_descriptor;
    typedef typename Refs::size_type                                     size_type;
    typedef typename Refs::Dart_const_descriptor                         Dart_const_descriptor;
    typedef typename Refs::Helper                                        Helper;
    typedef WithId                                                       Has_id;
    typedef Dart_operations_<typename Refs::Concurrent_tag, Self> Dart_operations;
    using Type_for_compact_container=typename Refs::Type_for_compact_container;


    /// Typedef for attributes
    template<int i>
    struct Attribute_descriptor: public Refs::template Attribute_descriptor<i>
    {};
    template<int i>
    struct Attribute_const_descriptor:
      public Refs::template Attribute_const_descriptor<i>
    {};

    /// The number of used marks.
    static const size_type NB_MARKS = Refs::NB_MARKS;

    /// The dimension of the combinatorial map.
    static const unsigned int dimension = d;

    Type_for_compact_container for_compact_container() const
    { return mf[0].for_compact_container(); }
    void for_compact_container(Type_for_compact_container p)
    { mf[0].for_compact_container(p); }

    Dart_descriptor get_f(unsigned int i) const
    {
      assert(i<=dimension);
      return mf[i];
    }

    bool operator==(const Self& other) const
    {
      if(mmarks!=other.mmarks ||
         mattribute_descriptors!=other.mattribute_descriptors)
      { return false; }
      for(unsigned int i=0; i<=dimension; ++i)
      { if(mf[i]!=other.mf[i]) { return false; }}
      return true;
    }

  protected:
    /** Default constructor: no real initialisation,
     *  because this is done in the combinatorial map class.
     */
    Dart_without_info()
    {}

    /** Copy constructor:
     * @param adart a dart.
     */
    Dart_without_info(const Dart_without_info& other) :
      mattribute_descriptors(other.mattribute_descriptors)
    {
      m_operations.copy_marks(other.mmarks, mmarks);

      for (unsigned int i=0; i<=dimension; ++i)
      { mf[i]=other.mf[i]; }
    }

    Self& operator=(const Self& other)
    {
      m_operations.copy_marks(other.mmarks, mmarks);
      mattribute_descriptors=other.mattribute_descriptors;
      for (unsigned int i=0; i<=dimension; ++i)
      { mf[i]=other.mf[i]; }
      return *this;
    }

    /** Return the mark value of a given mark number.
     * @param amark the mark number.
     * @return the value for this number.
     */
    bool get_mark(size_type amark) const
    {
      CGAL_assertion(amark>=0 && amark<NB_MARKS);

      return mmarks[amark];
    }

    /** Set the mark of a given mark number to a given value.
     * @param amark the mark number.
     * @param AValue the value.
     */
    void set_mark(size_type amark, bool avalue) const
    {
      CGAL_assertion(amark>=0 && amark<NB_MARKS);
      mmarks.set(amark, avalue);
    }
    /** Flip the mark of a given mark number.
     * @param amark the mark number.
     */
    void flip_mark(size_type amark) const
    {
      CGAL_assertion(amark>=0 && amark<NB_MARKS);
      mmarks.flip(amark);
    }

    /** Return all the marks of this dart.
     * @return the marks.
     */
     Bitset_type get_marks() const
    { return mmarks; }

    /** Set simultaneously all the marks of this dart to a given value.
     * @param amarks the value of the marks.
     */
     void set_marks(const Bitset_type& amarks) const
    {
         m_operations.copy_marks(amarks, mmarks);
     }

    /// @return a descriptor on the i-attribute
    template<int i>
    typename Attribute_descriptor<i>::type attribute()
    {
      CGAL_static_assertion_msg(Helper::template Dimension_index<i>::value>=0,
                     "attribute<i> called but i-attributes are disabled.");
      return std::get<Helper::template Dimension_index<i>::value>
        (mattribute_descriptors);
    }
    template<int i>
    typename Attribute_const_descriptor<i>::type attribute() const
    {
      CGAL_static_assertion_msg(Helper::template Dimension_index<i>::value>=0,
                     "attribute<i> called but i-attributes are disabled.");
      return std::get<Helper::template Dimension_index<i>::value>
        (mattribute_descriptors);
    }

  protected:
    /// Neighboors for each dimension +1 (from 0 to dimension).
    Dart_descriptor mf[dimension+1];

    /// Values of Boolean marks.
    mutable Bitset_type mmarks;

    /// Attributes enabled
    typename Helper::Attribute_descriptors mattribute_descriptors;

    Dart_operations m_operations;
  };

  // Dart definition with an info;
  //  (there is a specialization below when Info_==void)
  template <unsigned int d, typename Refs, typename Info_=void,
            class WithID=Tag_false>
  struct Dart : public Dart_without_info<d, Refs, WithID>
  {
  public:
    using Base=Dart_without_info<d, Refs, WithID>;

    template <class, class, class, class>
    friend class Compact_container;

    template <class, class>
    friend class Concurrent_compact_container;

    template <class, class, class, class>
    friend class Compact_container_with_index;

    template<unsigned int, class, class>
    friend class Combinatorial_map_storage_1;

    template<unsigned int, class, class>
    friend class Combinatorial_map_storage_with_index;

    template<unsigned int, class, class>
    friend class Generalized_map_storage_1;

    template<unsigned int, class, class>
    friend class Generalized_map_storage_with_index;

    template<unsigned int, unsigned int, class, class, class>
    friend class CMap_linear_cell_complex_storage_1;

    template<unsigned int, unsigned int, class, class, class>
    friend class CMap_linear_cell_complex_storage_with_index;

    template<unsigned int, unsigned int, class, class, class>
    friend class GMap_linear_cell_complex_storage_1;

    template<unsigned int, unsigned int, class, class, class>
    friend class GMap_linear_cell_complex_storage_with_index;

    typedef Dart<d, Refs, Info_, WithID> Self;
    typedef Info_                        Info;

    bool operator==(const Self& other) const
    { return Base::operator==(other) && minfo==other.minfo; }

  protected:
    /** Default constructor: no real initialisation,
     *  because this is done in the combinatorial or generalized map class.
     */
    Dart()=default; //  default => zero-initializing built-in types

    Dart(const Info_& info) : minfo(info)
    {}

  protected:
    Info_& info()
    { return minfo; }
    const Info_& info() const
    { return minfo; }

  protected:
    Info minfo;
  };

  // Specialization of Dart class when info==void
  template <unsigned int d, typename Refs, class WithID>
  struct Dart<d, Refs, void, WithID> : public Dart_without_info<d, Refs, WithID>
  {
  public:
    typedef CGAL::Void Info;
  };

  // Specialization of Dart class when info==CGAL::Void
  template <unsigned int d, typename Refs, class WithID>
  struct Dart<d, Refs, CGAL::Void, WithID> : public Dart_without_info<d, Refs, WithID>
  {
  public:
    typedef CGAL::Void Info;
  };

} // namespace CGAL

#endif // CGAL_DART_H //
// EOF //
