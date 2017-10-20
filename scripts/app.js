/*
*  SPA UI
*/ 

Vue.use(Buefy.default);
Vue.use(VueTruncate);

//
// Routes
//
var dashboard = {
	template: '#dashboard-templ',
	data: function (){
		return {
			activeTab: 0,
			loading: false,
			addItemActive: false,
			name: '',
			desc: '',
			price: '',
			catValue:'',
			inventory:'',
			categories: [ 'Electronics', 'Home' ],
			myItems: [],
			selected: { },
			// cart
			cart: [],
			cartCheck: false,
			cartChecked: []
		}
	},
	computed: {
		cartTotal: function() {
			if (this.$parent.user == null || this.cart.length == 0)
			return 0;
			else {
				var sum = 0; 
				this.cart.forEach( (item) => sum += item.count * item.price );
				return sum;
			}
		}
	},
	mounted: function() {
		if (this.$parent.user != null) {
			this.loadItems();
			this.loadCart();
		}
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
		loadCart: function() {
			var me = this;
			var _id = this.$parent.user._id;
			axios.post('/cgi-bin/cart', {
				method: 'get',
				user_id: _id,
			}).then(function(resp) {
				if (resp.data.success)
				me.cart = resp.data.items;
				else
				me.$toast.open('Error fetching cart');
			});
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
								me.closeModal();
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
					console.error("Error fetching request");
				}
			};
			if (app.user == null) {
				this.$toast.open('Sign In First');
				return;
			}
			xhr.send(oData);
		},
		clearSelectedItems: function() {
			var me = this;
			var sel_id = (this.selected)._id;
			var msg = 'Delete ' + this.selected.name + ' from items?';
			this.$dialog.confirm({
				message: msg,
				onConfirm: function() {
					axios.post('/cgi-bin/items', {
						method: 'remove',
						item: {
							_id: sel_id
						}
					}).then(function(resp){
						if (resp.data.success) {
							me.$toast.open('Deletion successful');
							me.myItems = me.myItems.filter((item) => item._id !== sel_id);
							me.closeModal();
						} else {
							me.$toast.open('Error deleting item');
						}	
					}).catch(console.error);
				}
			})
		},
		removeCartItems: function(e, curr) {
			var me = this
			if (typeof curr == "undefined") {
				curr = 0;
				this.cartCheck = false;
			}

			if (curr == this.cartChecked.length) {
				this.$toast.open('Items deleted from cart.');
				this.cart = this.cart.filter(function(item) {
					for (var i = 0;i < me.cartChecked.length;i++)
						if (me.cartChecked[i]._id == item._id)
							return false;
					return true;
				});
			}

			var u_id = this.$parent.user._id;
			axios.post('/cgi-bin/cart', {
				method: 'remove',
				user_id: u_id,
				product_id: me.cartChecked[curr]._id
			}).then(function(resp) {
				if (resp.data.success) {
					if (curr < me.cartChecked.length) {
						me.removeCartItems(e, curr+1);
					}
				}
			}).catch(console.error);;
		},
		checkout: function() {

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
			loading: false
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
		addToCart: function(e) {
			var index = parseInt(e.currentTarget.getAttribute('index'));
			var item = this.items[index];
			var user = this.$parent.user;
			var me = this;
			if (user == null) {
				this.$toast.open('Please sign in to use your cart');
			}
			axios.post('/cgi-bin/cart', {
				method: 'add',
				user_id: user._id,
				product_id: item._id,
				count: 1
			}).then(function(resp){
				if(resp.data.success) {	
					me.$toast.open('Added ' + item.name + ' to cart');
				} else {
					me.$toast.open('Some error occurred');
				}
			}).catch(console.error);
		},
		moreInfo: function(e) {
			this.selected = parseInt(e.currentTarget.getAttribute('item-id'));
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
	},
	methods: {
		plusCart: function() {
			user = this.$parent.user;
			item = this.item;
			if( user == null) {
				this.$toast.open('Please sign in to use your cart');
				return;
			}
			axios.post('/cgi-bin/cart', {
				method: 'add',
				user_id: user._id,
				product_id: item._id,
				count: 1
			}).then(function(resp){
				if(resp.data.success) {	
					me.$toast.open('Added ' + item.name + ' to cart');
				} else {
					me.$toast.open('Some error occurred');
				}
			}).catch(console.error);
		}
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
		},
		onSelect: function(op) {
			this.$parent.$router.push('/items/' + op._id);
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
			var xApp = this.$parent.$parent, me = this;
			if (this.$refs.emailField.isValid) {
				axios.post('/cgi-bin/login/', {
					register: this.register,
					email: this.email,
					pass: this.pass,
					type: this.typeOption - 1
				})
				.then(function (response) {
					if (response.data.success) {
						xApp.user = response.data.user;
						xApp.loggedIn = true;
						localStorage.setItem("user", JSON.stringify(xApp.user));
						me.closeModal();
					} else {
						xApp.$toast.open('Invalid Email/Password');
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

