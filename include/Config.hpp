#pragma once
#include <yaml-cpp/yaml.h>
#include "Includes.hpp"
#include "Utilities.hpp"

class ConfigParser;

class Config
{
    friend class ConfigParser;

    static constexpr char MESH_NAME_NO_EXTENSION[] = "mesh";
    static constexpr char OUPUT_DIR[] = "output";
    static constexpr std::array<const char *, 2> valid_mesh_extensions = {"su2", "nf"};
    bool valid_mesh_name(const string &name) const;

    /*Options specified when reading mesh*/

    Index N_NODES{0},
        N_TETS{0};

    Index N_INTERIOR_CELLS{0},
        N_TOTAL_CELLS{0},
        N_INTERIOR_FACES{0},
        N_TOTAL_FACES{0};
    bool grid_metrics_set{false};

    /*Options specified by input file*/

    string mesh_extension;
    string sim_dir;

    map<string, BoundaryType> map_patch_BC; // map from each patch to the bc type applied

    MainSolverType main_solver_type{0};

    TimeIntegrationType time_integration_type{0};

    TimeScheme time_scheme{0};

    SpatialOrder spatial_order{0};

    GradientScheme grad_scheme{0};

    InviscidFluxScheme inv_flux_scheme{0};

    Limiter limiter{0};

    InitialConditionOption initial_cond_option{0};

    size_t n_timesteps{0};

    size_t write_stride{0};

    size_t timestep{0};

    Scalar delta_time{0.0};

    Scalar time{0.0};

    Scalar CFL{0.0};

    bool grid_motion{false};

    Scalar primvars_inf[N_EQS_EULER]{0.0}; // May be used for setting initial values or boundary conditions

    bool check_if_physical{false};

    Time start_time; // Used for measuring time

public:
    /*Marking constructor explicit to not allow for errors of the type:
    calling foo(string), where for instance foo is defined as void foo(const Config &config);
    to disallow implicitly constructing a new Config object*/
    explicit Config(string config_filename);
    Config(const Config &other) = delete;
    Config operator=(const Config &other) = delete;

    void start_counter() { start_time = Clock::now(); }
    string get_elapsed_time() const;

    Index get_N_NODES() const
    {
        return N_NODES;
    }
    Index get_N_TETS() const { return N_TETS; }
    Index get_N_INTERIOR_CELLS() const { return N_INTERIOR_CELLS; }
    Index get_N_TOTAL_CELLS() const { return N_TOTAL_CELLS; }
    Index get_N_GHOST_CELLS() const { return N_TOTAL_CELLS - N_INTERIOR_CELLS; }
    Index get_N_INTERIOR_FACES() const { return N_INTERIOR_FACES; }
    Index get_N_TOTAL_FACES() const { return N_TOTAL_FACES; }
    Index get_N_BOUNDARY_FACES() const { return N_TOTAL_FACES - N_INTERIOR_FACES; }

    void set_grid_metrics(Index N_NODES,
                          Index N_INTERIOR_CELLS,
                          Index N_TOTAL_CELLS,
                          Index N_INTERIOR_FACES,
                          Index N_TOTAL_FACES);

    string get_mesh_name() const { return string(MESH_NAME_NO_EXTENSION) + "." + mesh_extension; }
    const string &get_sim_dir() const { return sim_dir; }
    string get_output_dir() const { return sim_dir + string(OUPUT_DIR) + "/"; }
    string get_mesh_filename_path() const { return sim_dir + get_mesh_name(); }
    string get_unsteady_vtk_filename() const { return get_output_dir() + "n=" + std::to_string(timestep) + ".vtk"; }

    BoundaryType get_boundary_type(const string &patch_name) const;

    bool input_file_contains_patch_name(const string &patch_name) const { return map_patch_BC.count(patch_name) == 1; }

    MainSolverType get_main_solver_type() const { return main_solver_type; }
    void set_main_solver_type(MainSolverType val) { main_solver_type = val; }

    TimeIntegrationType get_time_integration_type() const { return time_integration_type; }

    TimeScheme get_time_scheme() const { return time_scheme; }
    void set_time_scheme(TimeScheme val) { time_scheme = val; }

    GradientScheme get_grad_scheme() const { return grad_scheme; }
    void set_grad_scheme(GradientScheme val) { grad_scheme = val; }

    SpatialOrder get_spatial_order() const { return spatial_order; }
    void set_spatial_order(SpatialOrder val) { spatial_order = val; }

    InviscidFluxScheme get_inv_flux_scheme() const { return inv_flux_scheme; }
    void set_inv_flux_scheme(InviscidFluxScheme val) { inv_flux_scheme = val; }

    Limiter get_limiter() const { return limiter; }
    void set_limiter(Limiter val) { limiter = val; }

    InitialConditionOption get_initial_cond_option() const { return initial_cond_option; }
    void set_initial_cond_option(InitialConditionOption val) { initial_cond_option = val; }

    size_t get_n_timesteps() const { return n_timesteps; }

    size_t get_timestep() const { return timestep; }
    void set_timestep(size_t val) { timestep = val; }

    Scalar get_time() const { return time; }
    void set_time(Scalar val) { time = val; }

    Scalar get_delta_time() const { return delta_time; }
    void set_delta_time(Scalar val) { delta_time = val; }

    Scalar get_CFL() const { return CFL; }
    void set_CFL(Scalar val) { CFL = val; }

    size_t get_write_stride() const { return write_stride; }

    bool get_grid_motion() const { return grid_motion; }
    void set_grid_motion(bool val) { grid_motion = val; }

    Scalar get_primvars_inf(ShortIndex i_var) const
    {
        assert(i_var < N_EQS_EULER);
        return primvars_inf[i_var];
    }

    const Scalar *get_primvars_inf() const { return primvars_inf; }

    void set_primvars_inf(ShortIndex i_var, Scalar val)
    {
        assert(i_var < N_EQS_EULER);
        primvars_inf[i_var] = val;
    }

    bool check_physical_validity() const { return check_if_physical; }
};
