module perf_helper_mod
  use, intrinsic :: iso_c_binding
  implicit none

  interface
    subroutine perf_initialize() bind(C, name="f_perf_initialize")
    end subroutine perf_initialize

    subroutine perf_start_section(section) bind(C, name="f_perf_start_section")
      import :: c_int
      integer(c_int), intent(in) :: section
    end subroutine perf_start_section

    subroutine perf_stop_section(section) bind(C, name="f_perf_stop_section")
      import :: c_int
      integer(c_int), intent(in) :: section
    end subroutine perf_stop_section

    subroutine perf_finalize() bind(C, name="f_perf_finalize")
    end subroutine perf_finalize
  end interface

end module perf_helper_mod
