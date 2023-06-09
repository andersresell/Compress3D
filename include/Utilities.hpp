#pragma once
#include "Includes.hpp"
#include "containers/StaticContainer.hpp"

constexpr ShortIndex N_TET_NODES{4};
constexpr ShortIndex N_TRI_NODES{3};
constexpr ShortIndex N_TET_FACES{4};

constexpr ShortIndex N_EQS_EULER{N_DIM + 2};

namespace primvars_index
{
    static constexpr ShortIndex Density{0};
    static ShortIndex Velocity(ShortIndex i)
    {
        assert(i < N_DIM);
        return i + 1;
    }
    static constexpr ShortIndex Pressure{4};

}

// --------------------------------------------------------------------
// Some enums that specify solver behaviour and corresponding string -> enum maps for some
// --------------------------------------------------------------------
enum class MainSolverType
{
    Euler
};

const map<string, MainSolverType> main_solver_from_string{
    {"Euler", MainSolverType::Euler}};

enum class SolverType
{
    Euler
}; // The main solver can (but need not) be comprised of multiple sub solvers

const map<SolverType, string> string_from_solver_type{
    {SolverType::Euler, "Euler"}};

enum class TimeIntegrationType
{
    Explicit,
    Implicit
};

enum class TimeScheme
{
    ExplicitEuler,
    TVD_RK3
};

const map<string, TimeScheme> time_scheme_from_string{
    {"ExplicitEuler", TimeScheme::ExplicitEuler},
    {"TVD_RK3", TimeScheme::TVD_RK3}};

enum class SpatialOrder
{
    First,
    Second
};

const map<string, SpatialOrder> spatial_order_from_string{
    {"First", SpatialOrder::First},
    {"Second", SpatialOrder::Second}};

enum class GradientScheme
{
    GreenGauss
};

const map<string, GradientScheme> gradient_scheme_from_string{
    {"GreenGauss", GradientScheme::GreenGauss}};

enum class InviscidFluxScheme
{
    Rusanov,
    HLLC
};

const map<string, InviscidFluxScheme> inviscid_flux_scheme_from_string{
    {"Rusanov", InviscidFluxScheme::Rusanov},
    {"HLLC", InviscidFluxScheme::HLLC}};

enum class Limiter
{
    NONE,
    Barth
};

const map<string, Limiter> limiter_from_string{
    {"NONE", Limiter::NONE},
    {"Barth", Limiter::Barth}};

enum class BoundaryType
{
    NoSlipWall,
    SlipWall,
    FarField
};

const map<string, BoundaryType> boundary_type_from_string{
    {"NoSlipWall", BoundaryType::NoSlipWall},
    {"SlipWall", BoundaryType::SlipWall},
    {"FarField", BoundaryType::FarField}};

enum class InitialConditionOption
{
    Freestream
};

const map<string, InitialConditionOption> initial_condition_option_from_string{
    {"Freestream", InitialConditionOption::Freestream}};

namespace standard_air
{
    constexpr Scalar gas_constant{287.058};
    constexpr Scalar gamma{1.4};
    constexpr Scalar density{1.225};
    constexpr Scalar pressure{101325.0};
    constexpr Scalar temperature = pressure / (density * gas_constant);
}

namespace geom
{
    /*MAYBE IMPLEMENT THIS LATER TO IMRPOVE EFFICIENCY!*/

    /*A strucuture of arrays (AOS) containing the faces and their required properties*/
    /*class Faces
    {
        struct CellIndices
        {
            Index i, j;
            bool operator<(const Face &rhs) const
            {
                if (i != rhs.i)
                    return i < rhs.i;

                assert(j != rhs.j); // This would imply that cell indices are identical

                return j < rhs.j;
            }
        };

        Vector<CellIndices> cell_indices;
        Vector<Vec3> normal_areas;
        Vector<Vec3> centroid_to_face_i;
        Vector<Vec3> centroid_to_face_j;

    public:
        void resize(Index N_FACES);

        Index get_cell_i(Index face_index) const { return cell_indices[face_index].i; }
        Index get_cell_j(Index face_index) const { return cell_indices[face_index].i; }
        const Vec3 &get_normal_area(Index face_index) const { return normal_areas[face_index]; }
        const Vec3 &get_centroid_to_face_i(Index face_index) const { return centroid_to_face_i[face_index]; }
        const Vec3 &get_centroid_to_face_j(Index face_index) const { return centroid_to_face_j[face_index]; }

        void add_face(Index i, Index j, Vec3 S_ij, Vec3 r_im, Vec3 r_jm);

        /*Sort the faces with repsect to i and j, i having first priority
        void sort_faces(Index first_face, Index last_face)
        {
            CellIndices ci;
            Vect
            std::sort(cell_indices.begin() + first_face, cell_indices.begin() + last_face);
        }
    };*/

    struct Face
    {
        Face(Index i, Index j, Vec3 S_ij, Vec3 r_im, Vec3 r_jm) : S_ij{S_ij}, i{i}, j{j}, r_im{r_im}, r_jm{r_jm} {}
        Face(Index i, Index j) : i{i}, j{j} {}
        Vec3 S_ij;       // Area normal vector from cell i to j
        Index i, j;      // Indices of cell i and j
        Vec3 r_im, r_jm; // vectors from each cell center to the face centroid
        friend std::ostream &operator<<(std::ostream &os, const Face &f)
        {
            os << "(i,j) = (" << f.i << "," << f.j << "), S_ij: " + horizontal_string_Vec3(f.S_ij) + ", r_im: " + horizontal_string_Vec3(f.r_im) + ", r_jm: " + horizontal_string_Vec3(f.r_jm) << endl;
            return os;
        }

        bool operator<(const Face &rhs) const
        {
            if (i != rhs.i)
                return i < rhs.i;

            assert(j != rhs.j); // This would imply that cell indices are identical

            return j < rhs.j;
        }
    };

    struct Cell
    {
        Cell() = default;
        Cell(Scalar cell_volume, Vec3 centroid) : cell_volume{cell_volume}, centroid{centroid} {}
        Scalar cell_volume;
        Vec3 centroid;
        friend std::ostream &operator<<(std::ostream &os, const Cell &c)
        {
            os << "vol = " << c.cell_volume << ", centroid = " + horizontal_string_Vec3(c.centroid) << endl;
            return os;
        }
    };

    struct Patch
    {
        BoundaryType boundary_type;
        // Vector<Index> boundary_face_indices;
        Index N_FACES;
        Index FIRST_FACE;
    };

    /*Connectivity of a tetrahedron*/
    struct TetConnect final : public StaticContainer1D<Index, N_TET_NODES>
    {
        TetConnect() = default;
        TetConnect(Index a, Index b, Index c, Index d) : StaticContainer1D{a, b, c, d} {}
        Index &a() { return data[0]; }
        Index &b() { return data[1]; }
        Index &c() { return data[2]; }
        Index &d() { return data[3]; }
        Index a() const { return data[0]; }
        Index b() const { return data[1]; }
        Index c() const { return data[2]; }
        Index d() const { return data[3]; }
    };
    /*Connectivity of a triangle*/
    struct TriConnect : public StaticContainer1D<Index, N_TRI_NODES>
    {
        TriConnect() = default;
        TriConnect(Index a, Index b, Index c) : StaticContainer1D{a, b, c} {}
        Index &a() { return data[0]; }
        Index &b() { return data[1]; }
        Index &c() { return data[2]; }
        Index a() const { return data[0]; }
        Index b() const { return data[1]; }
        Index c() const { return data[2]; }
    };

    struct SortedTriConnect : public TriConnect
    {
        SortedTriConnect(const TriConnect &tc) : TriConnect{tc}
        {
            this->sort();
        }
        bool operator<(const SortedTriConnect &rhs) const
        {
            for (ShortIndex i{0}; i < N_TRI_NODES; i++)
                if (data[i] != rhs.data[i])
                    return data[i] < rhs.data[i];
            return false;
        }
    };

    /*Holds name and triangles of a boundary patch*/
    struct TriPatchConnect
    {
        string patch_name;
        Vector<TriConnect> triangles;
    };

    // returns the node connectivity of face_k from the node connectivity of tetraheder
    TriConnect tet_face_connectivity(TetConnect tc, ShortIndex face_k);

    /*Astract face geometry class*/
    struct Facegeom
    {
        Vector<Vec3> nodes;
        Facegeom(){};
        virtual Vec3 calc_area_normal() const = 0;
        virtual Vec3 calc_centroid() const = 0;
    };

    struct Triangle final : Facegeom
    {
        Triangle(Vec3 a, Vec3 b, Vec3 c) : Facegeom() { nodes = {a, b, c}; }
        Vec3 calc_area_normal() const final;
        Vec3 calc_centroid() const final;
    };

    /*Abstract volume geometry class*/
    struct Polyhedra
    {
        Vector<Vec3> nodes;
        virtual Scalar calc_volume() const = 0;
        virtual Vec3 calc_centroid() const = 0;
    };

    struct Tetrahedron final : Polyhedra
    {
        Tetrahedron(Vec3 a, Vec3 b, Vec3 c, Vec3 d) : Polyhedra() { nodes = {a, b, c, d}; }
        Scalar calc_volume() const final;
        Vec3 calc_centroid() const final;
    };

    void assign_face_properties(Face &face, const Facegeom &face_geom, const Vec3 &cell_center_i, const Vec3 &cell_center_j);

    Vec3 calc_ghost_centroid(Vec3 centroid_i, const Facegeom &boundary_face);
}