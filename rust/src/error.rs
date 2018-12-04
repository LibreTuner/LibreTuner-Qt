use std::cell::RefCell;
use std::os::raw::c_char;
use std::ptr;

use std::ffi::CString;

use libretuner::error::Error;

thread_local!{
    static LAST_ERROR: RefCell<Option<Error>> = RefCell::new(None);
}

pub fn set_last_error(error: Error) {
	LAST_ERROR.with(|f| {
		*f.borrow_mut() = Some(error);
	});
}

#[no_mangle]
pub extern "C" fn get_last_error_string() -> *const c_char {
	LAST_ERROR.with(|f| -> *const c_char {
		let borrow = f.borrow();
		let raw = match *borrow {
			Some(ref r) => r,
			None => return ptr::null(),
		};

		let raw = match CString::new(raw.to_string()) {
			Ok(r) => r,
			Err(err) => {
				println!("Failed to get CString from error: {}. {}", raw.to_string(), err);
				return ptr::null();
			}
		};

		raw.into_raw()
	})
	
}