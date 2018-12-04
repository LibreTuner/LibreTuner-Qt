pub mod error;

use libretuner::app::App;
use libretuner::tuneutils::{
	link::{PlatformLink, DataLink},
	download::{Downloader, DownloadCallback},
};

use std::os::raw::{c_char, c_void};
use std::ptr;
use std::ffi::{CStr, CString};
use std::mem;


#[no_mangle]
pub extern "C" fn create_app() -> *mut App {
	let app = App::new();

	match app {
		Ok(app) => Box::into_raw(Box::new(app)),
		Err(err) => {
			// Log error
			error::set_last_error(err);
			ptr::null_mut()
		},
	}
}



#[no_mangle]
pub unsafe extern "C" fn app_get_datalink(app: *mut App, id: usize) -> *mut c_void {
	// We cast everything to *mut c_void. It's dirty, but std::ptr::null_mut() is broken for ?Sized type
	match (*app).get_datalink(id) {
		Ok(res) => Box::into_raw(res) as *mut c_void,
		Err(err) => {
			error::set_last_error(err);
			ptr::null_mut()
		}
	}
}



#[no_mangle]
pub unsafe extern "C" fn app_list_datalinks(app: *mut App, cb: fn(*const *const c_char, usize, *const c_void), userdata: *const c_void) {
	let mut links = Vec::with_capacity((*app).avail_links.len());

	for link in &(*app).avail_links {
		links.push(CString::new(format!("{} {}", link.typename(), link.description())).unwrap());
	}

	let mut raw_links = Vec::with_capacity(links.len());
	for link in &links {
		raw_links.push(link.as_ptr());
	}

	cb(raw_links.as_ptr(), links.len(), userdata);
}



#[no_mangle]
pub unsafe extern "C" fn app_list_platforms(app: *mut App, cb: fn(*const *const c_char, *const *const c_char, usize, *const c_void), userdata: *const c_void) {
	let platforms: Vec<(CString, CString)> = (*app).list_platforms().iter().map(|(name, id)| (CString::new(*name).unwrap(), CString::new(*id).unwrap())).collect();

	let mut names = Vec::with_capacity(platforms.len());
	let mut ids = Vec::with_capacity(platforms.len());
	for (name, id) in &platforms {
		names.push(name.as_ptr());
		ids.push(id.as_ptr());
	}

	cb(names.as_ptr(), ids.as_ptr(), platforms.len(), userdata);
}


#[no_mangle]
pub unsafe extern "C" fn app_create_platform_link(app: *mut App, datalink: usize, platform: *const c_char) -> *mut PlatformLink {
	let raw = CStr::from_ptr(platform);
	let link = (*app).create_platform_link(datalink, raw.to_str().unwrap());

	match link {
		Ok(l) => Box::into_raw(Box::new(l)),
		Err(err) => {
			error::set_last_error(err);
			ptr::null_mut()
		}
	}
}

#[no_mangle]
pub unsafe extern "C" fn platformlink_downloader(link: *mut PlatformLink) -> *mut c_void {
	match (*link).downloader() {
		Some(d) => Box::into_raw(d) as *mut c_void,
		None => {
			ptr::null_mut()
		}
	}
}

// pub fn download(&mut self, link: &PlatformLink, id: &str, name: &str, callback: &DownloadCallback) -> Result<()> {
#[no_mangle]
pub unsafe extern "C" fn app_download(app: *mut App, link: *mut PlatformLink, id: *const c_char, name: *const c_char, callback: fn(f32, *const c_void), userdata: *const c_void) -> bool {
	println!("Called app_download");
	let id = CStr::from_ptr(id).to_str().unwrap();
	let name = CStr::from_ptr(name).to_str().unwrap();

	let res = (*app).download(&(*link), id, name, &DownloadCallback::with(move |progress| {
		callback(progress, userdata);
	}));

	match res {
		Ok(_) => true,
		Err(err) => {
			error::set_last_error(err);
			false
		}
	}
}

#[no_mangle]
pub unsafe extern "C" fn downloader_download(downloader: *mut Downloader, download_callback: fn(*const u8, usize, *const c_void), progress_callback: fn(f32, *const c_void), userdata: *const c_void) -> bool {
	let res = (*downloader).download(&DownloadCallback::with(move |progress| {
		progress_callback(progress, userdata);
	}));

	match res {
		Ok(r) => {
			download_callback(r.data.as_ptr(), r.data.len(), userdata);
			true
		},
		Err(err) => {
			error::set_last_error(libretuner::error::Error::TuneUtils(err));
			false
		}
	}
}

#[no_mangle]
pub unsafe extern "C" fn drop_downloader(downloader: *mut Downloader) {
	println!("Dropping downloader");
	mem::drop(Box::from_raw(downloader));
}

#[no_mangle]
pub unsafe extern "C" fn drop_platformlink(link: *mut PlatformLink) {
	mem::drop(Box::from_raw(link));
}

#[no_mangle]
pub unsafe extern "C" fn drop_datalink(datalink: *mut DataLink) {
	mem::drop(Box::from_raw(datalink));
}



#[no_mangle]
pub unsafe extern "C" fn drop_app(app: *mut App) {
	mem::drop(Box::from_raw(app));
}



#[no_mangle]
pub unsafe extern "C" fn roms_len(app: *mut App) -> usize {
	(*app).roms.roms.len() as usize
}



#[no_mangle]
pub unsafe extern "C" fn tunes_len(app: *mut App, rom_id: usize) -> usize {
	if rom_id > (*app).roms.roms.len() {
		return 0;
	}

	let rom = &(*app).roms.roms[rom_id];
	(*app).tunes.tunes.iter().filter(|t| t.rom_id == rom.id).count()
}



#[no_mangle]
pub unsafe extern "C" fn rom_name(app: *mut App, rom_id: usize) -> *const c_char {
	if rom_id > (*app).roms.roms.len() {
		return ptr::null();
	}
	let rom = &(*app).roms.roms[rom_id];

	let raw = match CString::new(&rom.name[..]) {
		Ok(r) => r,
		Err(err) => {
			println!("Failed to get CString from error: {}. {}", rom.name, err);
			return ptr::null();
		}
	};

	raw.into_raw()
}



#[no_mangle]
pub unsafe extern "C" fn get_tune_id(app: *mut App, rom_id: usize, tune_off: usize) -> isize {
	if rom_id > (*app).roms.roms.len() {
		return -1;
	}
	let rom = &(*app).roms.roms[rom_id];

	let mut rom_offset = 0;
	for (id, t) in (*app).tunes.tunes.iter().enumerate() {
		if t.rom_id == rom.id {
			rom_offset += 1;
		}
		if rom_offset == tune_off {
			return id as isize;
		}
	}
	return -1;
}



#[no_mangle]
pub unsafe extern "C" fn tune_name(app: *mut App, tune_id: usize) -> *const c_char {
	if tune_id > (*app).tunes.tunes.len() {
		return ptr::null();
	}
	let tune = &(*app).tunes.tunes[tune_id];

	let raw = match CString::new(&tune.name[..]) {
		Ok(r) => r,
		Err(err) => {
			println!("Failed to get CString from error: {}. {}", tune.name, err);
			return ptr::null();
		}
	};

	raw.into_raw()
}



#[no_mangle]
pub unsafe extern "C" fn drop_string(raw: *mut c_char) {
	mem::drop(Box::from_raw(raw));
}