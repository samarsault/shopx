/*
*  SPA UI
*/ 

Vue.use(Buefy.default);

//
// Routes
//
var dashboard = {
	template: '#dashboard-templ',
	data: function (){
		return {
			activeTab: 0,
			cart: [],
			loading: false,
			addItemActive: false,
			name: '',
			desc: '',
			price: '',
			catValue:'',
			inventory:'',
			categories: [ 'Electronics', 'Home' ],
			myItems: []
		}
	},
	mounted: function() {
		if (this.$parent.user != null)
		this.loadItems();
	},
	methods: {
		loadItems: function() {
			var me = this;
			console.log('Loading items..');
			axios.post('/cgi-bin/items', {
				method: 'get',
				item: {
					seller_id: me.$parent.user._id
				}
			}).then(function(resp) {
				me.myItems = resp.data.items;
			}).catch(console.error);
		},
		addItem: function() {
			var form = document.forms.namedItem("imgupload");
			var me = this;
			oData = new FormData(form);
			var xhr = new XMLHttpRequest();
			xhr.open("POST", "/cgi-bin/upload.cgi", true);
			xhr.onload = function(oEvent) {
				if (xhr.status == 200) {
					// success
					var data = JSON.parse(xhr.response);
					if (data.success) {
						// add item
						axios.post('/cgi-bin/items', {
							method: 'add',
							item: {
								name: me.name,
								seller_id: app.user._id,
								category: me.catValue,
								description: me.desc,
								price: parseInt(me.price),
								imageLink: data.url,
								discount: 0, // TODO: Add Discount
								inventory: parseInt(me.inventory)
							}
						}).then(function (resp){
							if (resp.data.success) {
								me.$toast.open('Item Added Successfully..')
							} else {
								me.$toast.open('Error adding item');
							}
						})
						.catch(function (err){
							console.error(err);
						});
					} else {
						me.$toast.open('Error uploading image')
					}
				} else {
				}
			};
			if (app.user == null) {
				this.$toast.open('Sign In First');
				return;
			}
			xhr.send(oData);
		},
		closeModal: function() {
			this.$refs.itemsModal.close();
		}
	}
};

var shop = {
	template: '#shop-templ',
	data: function() {
		return {
			items: [],
			loading: false,
			itVisible: false,
			selected: -1
		};
	},
	mounted: function() {
		this.load();
	},
	methods:  {
		load: function() {
			var me = this;
			axios.post('/cgi-bin/items', {
				method: 'all'
			}).then(function(resp) {
				if (resp.data.success) {
					me.items = resp.data.items;
				}
			}).catch(console.error);
		},
		moreInfo: function(e) {
			this.selected = parseInt(e.currentTarget.getAttribute('id'));
			this.itVisible = true;
		}
	}
};

var itemInfo = {
	template: '#item-info-templ',
	props: ['id'],
	data: function() {
		return {
			item: {}
		}
	},
	beforeMount: function() {
		var me = this;
		var id = parseInt(this.$route.params.id);
		axios.post('/cgi-bin/items', {
			method: 'get',
			item: {
				_id: id
			}
		}).then(function(resp){
			if (resp.data.success)
			me.item = resp.data.item;
			else
			console.log('Error!');
		}).catch(console.error);
	}
};

const router = new VueRouter({
	routes: [
		{ path: '/', component: { template: '#home-page'} },
		{ path: '/dashboard', component: dashboard },
		{ path: '/shop', component: shop },
		{ path: '/items/:id', component: itemInfo }
	]
});

//
// Components
//

// TODO: Implement Server Side Search
Vue.component('item-search', {
	template: '#item-search-templ',
	data: function() {
		return {
			isFetching: false,
			oData: [],
			data: [],
			name: '',
			selected: null,
			nameArr: [],
			fuse: null
		}
	},
	beforeMount: function() {
		var me = this;
		axios.post('/cgi-bin/items', {
			method: 'all'
		}).then(function(resp) {
			if (resp.data.success) {
				me.oData = resp.data.items;
				me.isFetching = false;
				me.fuse = new Fuse(resp.data.items, {
					shouldSort: true,
					threshold: 0.6,
					location: 0,
					distance: 100,
					maxPatternLength: 32,
					minMatchCharLength: 1,
					keys: [
						"name",
						"description"
					]
				});;
			}
		}).catch(console.error);
	},
	methods: {
		getData: function() {
			this.data = [];
			var me = this;
			if (me.oData.length >= 0) {
				this.data = this.fuse.search(this.name);
			}
		}
	}
});

Vue.component('auth', {
	template: '#auth-templ',
	data: function () {
		return {
			register: false,
			email: '',
			pass: '',
			typeOption: 0
		}
	},
	methods: {
		login: function() {
			var me = this;
			if (this.$refs.emailField.isValid) {
				axios.post('/cgi-bin/login/', {
					register: this.register,
					email: this.email,
					pass: this.pass,
					type: this.typeOption - 1
				})
				.then(function (response) {
					if (response.data.success) {
						me.$parent.user = response.data.user;
						me.$parent.loggedIn = true;
						localStorage.setItem("user", JSON.stringify(me.$parent.user));
						me.closeModal();
					}
				})
				.catch(function (error) {
					console.log(error);
				});
			}
		},
		closeModal: function() {
			this.$parent.close();
		}
	}
});

var app = new Vue({
	el: '#app',
	router: router,
	beforeMount: function() {
		var user = localStorage.getItem('user');
		if (user != null) {
			this.user = JSON.parse(user);
			this.loggedIn = true;
		}
	},
	data: {
		isLoginActive: false,
		loggedIn: false,
		user: null
	},
	methods: {
		logout: function () {
			localStorage.removeItem('user');
			this.user = null;
			this.loggedIn = false;
			// navigate to home page
			this.$router.push('/');
		}
	}
});

